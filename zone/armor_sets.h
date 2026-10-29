#pragma once

#include "common/dynamic_item_structs.h"
#include "common/types.h"

#include <unordered_map>
#include <vector>

// Armor sets: items in armor_set_items, bonuses in armor_set_bonuses (loaded at zone boot)
struct ArmorSetItemDef {
	uint32 item_id;
	uint32 set_id;
	uint32 class_mask;
};

struct ArmorSetBonusDef {
	uint32 set_id;
	uint32 min_pieces;
	uint32 class_mask;
	int32 stat_id;
	int32 stat_value;
	EQ::eDynamicAffixID affix = EQ::AFFIX_NONE; // set: a dynamic item affix code is used instead of stat_id
	uint16 spell_id   = 0;                      // set: the bonus is a spell instead (see spell_type)
	uint8  spell_type = 0;                      // ArmorSetSpellType
};

// armor_set_bonuses.spell_type
enum ArmorSetSpellType : uint8 {
	ArmorSetSpellWorn        = 1, // applied like an item worn effect
	ArmorSetSpellMeleeProc   = 2, // weapon proc while the bonus is active
	ArmorSetSpellSympathetic = 3, // spell-cast proc, checked like an item focus
};

extern std::unordered_map<uint32, ArmorSetItemDef> g_armor_set_item_map;
extern std::unordered_map<uint32, std::vector<ArmorSetBonusDef>> g_armor_set_bonuses_map;

void LoadArmorSetBonuses();
