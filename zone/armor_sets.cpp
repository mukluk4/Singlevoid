#include "zone/armor_sets.h"

#include "common/rulesys.h"
#include "common/strings.h"
#include "zone/client.h"
#include "zone/dynamic_items.h"
#include "zone/zonedb.h"

std::unordered_map<uint32, ArmorSetItemDef> g_armor_set_item_map;
std::unordered_map<uint32, std::vector<ArmorSetBonusDef>> g_armor_set_bonuses_map;

void LoadArmorSetBonuses() {
	g_armor_set_item_map.clear();
	g_armor_set_bonuses_map.clear();

	auto results_items = database.QueryDatabase("SELECT item_id, set_id, class_mask FROM armor_set_items");
	if (results_items.Success()) {
		for (auto row = results_items.begin(); row != results_items.end(); ++row) {
			ArmorSetItemDef item_def;
			item_def.item_id = atoi(row[0]);
			item_def.set_id = atoi(row[1]);
			item_def.class_mask = atoi(row[2]);
			g_armor_set_item_map[item_def.item_id] = item_def;
		}
	}

	auto results_bonuses = database.QueryDatabase("SELECT set_id, min_pieces, class_mask, stat_id, stat_value, affix, spell_id, spell_type FROM armor_set_bonuses");
	if (results_bonuses.Success()) {
		for (auto row = results_bonuses.begin(); row != results_bonuses.end(); ++row) {
			ArmorSetBonusDef bonus;
			uint32 set_id = atoi(row[0]);
			bonus.set_id = set_id;
			bonus.min_pieces = atoi(row[1]);
			bonus.class_mask = atoi(row[2]);
			bonus.stat_id = atoi(row[3]);
			bonus.stat_value = atoi(row[4]);
			bonus.spell_id   = static_cast<uint16>(atoi(row[6]));
			bonus.spell_type = static_cast<uint8>(atoi(row[7]));

			if (bonus.spell_id && (!IsValidSpell(bonus.spell_id) || bonus.spell_type < ArmorSetSpellWorn || bonus.spell_type > ArmorSetSpellSympathetic)) {
				LogError("armor_set_bonuses set [{}] has invalid spell [{}] type [{}], skipping", set_id, bonus.spell_id, bonus.spell_type);
				continue;
			}

			if (row[5] && row[5][0]) {
				bonus.affix = DynamicItemsManager::AffixFromCode(row[5]);
				if (bonus.affix == EQ::AFFIX_NONE) {
					LogError("armor_set_bonuses set [{}] has unknown affix [{}], skipping", set_id, row[5]);
					continue;
				}
			}

			g_armor_set_bonuses_map[set_id].push_back(bonus);
		}
	}

	LogInfo("Loaded {} armor set items and {} armor set bonus configurations.", g_armor_set_item_map.size(), g_armor_set_bonuses_map.size());
}

std::unordered_map<uint32, uint32> Client::CountEquippedArmorSetPieces() {
	std::unordered_map<uint32, uint32> equipped_set_counts;
	// All of the character's classes on multiclass servers, the single class otherwise
	uint32 player_class_bit = GetClassesBits();

	// Expanded slot check to include main armor, jewelry, and weapon slots
	const int16 equipment_slots[] = {
		// Main Armor (8)
		EQ::invslot::slotHead,
		EQ::invslot::slotChest,
		EQ::invslot::slotArms,
		EQ::invslot::slotWrist1,
		EQ::invslot::slotWrist2,
		EQ::invslot::slotHands,
		EQ::invslot::slotLegs,
		EQ::invslot::slotFeet,

		// Jewelry & Accessories (9)
		EQ::invslot::slotEar1,
		EQ::invslot::slotEar2,
		EQ::invslot::slotFace,
		EQ::invslot::slotNeck,
		EQ::invslot::slotShoulders,
		EQ::invslot::slotBack,
		EQ::invslot::slotFinger1,
		EQ::invslot::slotFinger2,
		EQ::invslot::slotWaist,

		// Weapons & Ranged (4)
		EQ::invslot::slotPrimary,
		EQ::invslot::slotSecondary,
		EQ::invslot::slotRange,
		EQ::invslot::slotCharm
	};

	// Process all slots in the list
	for (int16 slot_id : equipment_slots) {
		const EQ::ItemInstance* item = GetInv()[slot_id];
		if (!item) continue;

		uint32 item_id = item->GetID();
		auto it = g_armor_set_item_map.find(item_id);
		if (it != g_armor_set_item_map.end()) {
			const auto& set_item = it->second;
			// Validate class mask: allow if 0 (all classes) or if class bit matches
			if (set_item.class_mask == 0 || (set_item.class_mask & player_class_bit)) {
				equipped_set_counts[set_item.set_id]++;
			}
		}
	}

	return equipped_set_counts;
}

void Client::CheckArmorSetBonuses(StatBonuses* itembonuses) {
	if (!itembonuses) return;

	const auto equipped_set_counts = CountEquippedArmorSetPieces();
	uint32 player_class_bit = GetClassesBits();

	// Spell bonuses are rebuilt on every recalculation; weapon procs are synced after the loop
	std::set<uint16> wanted_procs;
	m_set_bonus_sympathetic.clear();

	// Process stats and state transitions for each active set
	for (const auto& [set_id, piece_count] : equipped_set_counts) {
		auto bonus_it = g_armor_set_bonuses_map.find(set_id);
		if (bonus_it == g_armor_set_bonuses_map.end()) continue;

		// Dynamically calculate tier based on database min_pieces thresholds instead of hardcoded >= 6
		uint32 new_tier = 0;
		for (const auto& bonus : bonus_it->second) {
			if (piece_count >= bonus.min_pieces) {
				uint32 tier_for_bonus = (bonus.min_pieces >= 8) ? 3 : (bonus.min_pieces >= 4) ? 2 : 1;
				if (tier_for_bonus > new_tier) {
					new_tier = tier_for_bonus;
				}
			}
		}
		uint32 current_tier = m_active_armor_set_tiers[set_id];

		
		// Trigger visuals and chat messages ONLY when transitioning between tiers
		if (new_tier != current_tier) {
			uint32 visual_spell_id = 0;

			// Custom:ArmorSetTierVisuals = "tier 1,tier 2,tier 3,tier lost" spell effect ids (0 = none)
			const auto visuals = Strings::Split(RuleS(Custom, ArmorSetTierVisuals), ',');
			auto visual = [&](size_t i) -> uint32 {
				if (i >= visuals.size()) {
					return 0;
				}
				std::string id = visuals[i];
				return Strings::ToUnsignedInt(Strings::Trim(id));
			};

			if (new_tier > current_tier) {
				// Gaining a tier
				visual_spell_id = visual(new_tier - 1);

				// Send upgrade message in chat (Yellow text = CC_Yellow / 15)
				this->Message(15, "You have unlocked Armor Set Tier %d!", new_tier);
			}
			else if (new_tier < current_tier) {
				// Losing a tier
				visual_spell_id = visual(3);

				if (new_tier > 0) {
					this->Message(15, "Your Armor Set has dropped to Tier %d.", new_tier);
				}
				else {
					this->Message(13, "You no longer meet the requirements for any Armor Set bonuses.");
				}
			}

			// Send visual particle effect to zone
			if (visual_spell_id > 0) {
				this->SendSpellEffect(
					this->GetID(),
					this->GetID(),
					visual_spell_id,
					0,
					1000,
					true
				);
			}
		}
		// Update the stored tier state for this set ID
		m_active_armor_set_tiers[set_id] = new_tier;

		// Apply Stat Bonuses
		for (const auto& bonus : bonus_it->second) {
			if (piece_count >= bonus.min_pieces) {
				if (bonus.class_mask == 0 || (bonus.class_mask & player_class_bit)) {
					if (bonus.affix != EQ::AFFIX_NONE) {
						DynamicItemsManager::ApplyCharacterAffix(bonus.affix, bonus.stat_value, itembonuses, &dynamic_item_bonuses);
						continue;
					}

					if (bonus.spell_id) {
						switch (bonus.spell_type) {
							case ArmorSetSpellWorn:
								ApplySpellsBonuses(bonus.spell_id, GetLevel(), itembonuses, 0, EQ::item::ItemEffectWorn);
								break;
							case ArmorSetSpellMeleeProc:
								wanted_procs.insert(bonus.spell_id);
								break;
							case ArmorSetSpellSympathetic:
								// Checked by GetSympatheticFocusEffect on each damage or heal cast
								m_set_bonus_sympathetic.push_back(bonus.spell_id);
								break;
						}
						continue;
					}

					switch (bonus.stat_id) {
						// --- BASIC STATS ---
					case 1: itembonuses->HP += bonus.stat_value; break;
					case 2: itembonuses->Mana += bonus.stat_value; break;
					case 3: itembonuses->AC += bonus.stat_value; break;
					case 4: itembonuses->ATK += bonus.stat_value; break;
					case 5: itembonuses->ManaRegen += bonus.stat_value; break;
					case 6: itembonuses->HPRegen += bonus.stat_value; break;
					case 7: itembonuses->haste += bonus.stat_value; break; // Lowercase 'haste'
					case 8: itembonuses->DamageShield += bonus.stat_value; break;

						// --- BASE ATTRIBUTES ---
					case 20: itembonuses->STR += bonus.stat_value; break;
					case 21: itembonuses->STA += bonus.stat_value; break;
					case 22: itembonuses->DEX += bonus.stat_value; break;
					case 23: itembonuses->AGI += bonus.stat_value; break;
					case 24: itembonuses->WIS += bonus.stat_value; break;
					case 25: itembonuses->INT += bonus.stat_value; break;
					case 26: itembonuses->CHA += bonus.stat_value; break;

						// --- HEROIC STATS ---
					case 30: itembonuses->HeroicSTR += bonus.stat_value; break;
					case 31: itembonuses->HeroicSTA += bonus.stat_value; break;
					case 32: itembonuses->HeroicDEX += bonus.stat_value; break;
					case 33: itembonuses->HeroicAGI += bonus.stat_value; break;
					case 34: itembonuses->HeroicWIS += bonus.stat_value; break;
					case 35: itembonuses->HeroicINT += bonus.stat_value; break;
					case 36: itembonuses->HeroicCHA += bonus.stat_value; break;

						// --- RESISTANCES ---
					case 60: itembonuses->MR += bonus.stat_value; break;
					case 61: itembonuses->FR += bonus.stat_value; break;
					case 62: itembonuses->CR += bonus.stat_value; break;
					case 63: itembonuses->PR += bonus.stat_value; break;
					case 64: itembonuses->DR += bonus.stat_value; break;

						// --- SKILL ACCURACY / CAP MODIFIERS ---
					case 85: itembonuses->skillmod[EQ::skills::SkillArchery] += bonus.stat_value; break;
					case 86: itembonuses->skillmod[EQ::skills::SkillBackstab] += bonus.stat_value; break;
					case 87: itembonuses->skillmod[EQ::skills::SkillFlyingKick] += bonus.stat_value; break;
					case 88: itembonuses->skillmod[EQ::skills::SkillFrenzy] += bonus.stat_value; break;

					default:
						break;
					}
				}
			}
		}
	}

	// Weapon procs from set bonuses: drop the ones no longer earned, (re)add the earned ones that are missing
	for (auto it = m_set_bonus_procs.begin(); it != m_set_bonus_procs.end();) {
		if (!wanted_procs.contains(*it)) {
			RemoveProcFromWeapon(*it);
			it = m_set_bonus_procs.erase(it);
		} else {
			++it;
		}
	}
	for (const auto proc : wanted_procs) {
		if (std::none_of(std::begin(SpellProcs), std::end(SpellProcs), [proc](const tProc &p) { return p.spellID == proc; })) {
			AddProcToWeapon(proc, false, 100);
		}
		m_set_bonus_procs.insert(proc);
	}
}
