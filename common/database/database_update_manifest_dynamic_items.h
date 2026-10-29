/*	EQEmu: EQEmulator

	Copyright (C) 2001-2026 EQEmu Development Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "common/database/database_update.h"

#include <vector>

// Dynamic (seeded) items and armor set bonuses. These keep their own version counter
// (db_version.dynamic_items_version, DYNAMIC_ITEMS_BINARY_DATABASE_VERSION), so they never collide
// with a server's own custom migrations. Add new entries at the end with the next version number.
std::vector<ManifestEntry> manifest_entries_dynamic_items = {
	ManifestEntry{
		.version = 1,
		.description = "2026_10_21_dynamic_items_tables",
		.check = "SHOW TABLES LIKE 'dynamic_affix_pool'",
		.condition = "empty",
		.match = "",
		.sql = R"(
CREATE TABLE IF NOT EXISTS `dynamic_items` (
  `item_id` INT UNSIGNED NOT NULL,
  `class_id` TINYINT UNSIGNED NOT NULL,
  `tier` TINYINT UNSIGNED NOT NULL DEFAULT 1,
  `min_affixes` TINYINT UNSIGNED NOT NULL DEFAULT 1,
  `max_affixes` TINYINT UNSIGNED NOT NULL DEFAULT 2,
  `stat_multiplier` FLOAT NOT NULL DEFAULT 1.0,
  `notes` VARCHAR(255) NULL,
  PRIMARY KEY (`item_id`)
);

CREATE TABLE IF NOT EXISTS `dynamic_affix_pool` (
  `class_id` TINYINT UNSIGNED NOT NULL,
  `affix` VARCHAR(32) NOT NULL,
  `guaranteed` TINYINT UNSIGNED NOT NULL DEFAULT 0,
  `weight` INT UNSIGNED NOT NULL DEFAULT 100,
  `min_value` INT NOT NULL DEFAULT 0,
  `max_value` INT NOT NULL DEFAULT 0,
  `slot_mask` INT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`class_id`, `affix`)
);

INSERT IGNORE INTO `dynamic_affix_pool` (`class_id`, `affix`, `guaranteed`, `weight`, `min_value`, `max_value`, `slot_mask`) VALUES
-- Warrior
(1, 'hp', 1, 0, 360, 440, 0),
(1, 'ac', 1, 0, 80, 104, 0),
(1, 'endurance', 1, 0, 290, 350, 0),
(1, 'heroic_str', 1, 0, 19, 25, 0),
(1, 'heroic_sta', 1, 0, 19, 25, 0),
(1, 'heroic_dex', 1, 0, 11, 15, 0),
(1, 'heroic_agi', 1, 0, 12, 15, 0),
(1, 'heroic_int', 1, 0, 5, 10, 0),
(1, 'heroic_wis', 1, 0, 5, 10, 0),
(1, 'skill_1hs_pct', 0, 100, 3, 12, 0),
(1, 'skill_2hs_pct', 0, 100, 3, 12, 0),
(1, 'skill_1hb_pct', 0, 100, 3, 12, 0),
(1, 'skill_2hb_pct', 0, 100, 3, 12, 0),
(1, 'skill_pierce_pct', 0, 100, 3, 12, 0),
(1, 'skill_defense_pct', 0, 100, 2, 8, 0),
(1, 'skill_parry_pct', 0, 100, 2, 8, 0),
(1, 'skill_riposte_pct', 0, 100, 2, 8, 0),
(1, 'bash_skill_pct', 0, 100, 3, 10, 0),
(1, 'bash_dmg_pct', 0, 100, 5, 18, 0),
-- Cleric
(2, 'hp', 1, 0, 300, 370, 0),
(2, 'ac', 1, 0, 80, 104, 0),
(2, 'mana', 1, 0, 360, 440, 0),
(2, 'heroic_str', 1, 0, 11, 17, 0),
(2, 'heroic_sta', 1, 0, 11, 17, 0),
(2, 'heroic_dex', 1, 0, 5, 10, 0),
(2, 'heroic_agi', 1, 0, 5, 10, 0),
(2, 'heroic_int', 1, 0, 5, 10, 0),
(2, 'heroic_wis', 1, 0, 19, 25, 0),
(2, 'skill_1hb_pct', 0, 100, 3, 12, 0),
(2, 'skill_2hb_pct', 0, 100, 3, 12, 0),
(2, 'skill_defense_pct', 0, 100, 2, 8, 0),
(2, 'heal_amount_pct', 0, 250, 3, 15, 0),
(2, 'spell_magic_pct', 0, 100, 3, 15, 0),
-- Paladin
(3, 'hp', 1, 0, 345, 422, 0),
(3, 'ac', 1, 0, 72, 94, 0),
(3, 'mana', 1, 0, 180, 220, 0),
(3, 'endurance', 1, 0, 145, 175, 0),
(3, 'heroic_str', 1, 0, 17, 23, 0),
(3, 'heroic_sta', 1, 0, 17, 23, 0),
(3, 'heroic_dex', 1, 0, 10, 14, 0),
(3, 'heroic_agi', 1, 0, 10, 14, 0),
(3, 'heroic_int', 1, 0, 5, 10, 0),
(3, 'heroic_wis', 1, 0, 8, 14, 0),
(3, 'skill_1hs_pct', 0, 100, 2, 9, 0),
(3, 'skill_2hs_pct', 0, 100, 2, 9, 0),
(3, 'skill_1hb_pct', 0, 100, 2, 9, 0),
(3, 'skill_2hb_pct', 0, 100, 2, 9, 0),
(3, 'skill_defense_pct', 0, 100, 2, 6, 0),
(3, 'skill_parry_pct', 0, 100, 2, 6, 0),
(3, 'skill_riposte_pct', 0, 100, 2, 6, 0),
(3, 'bash_skill_pct', 0, 100, 2, 8, 0),
(3, 'bash_dmg_pct', 0, 100, 4, 14, 0),
(3, 'heal_amount_pct', 0, 100, 1, 4, 0),
-- Ranger
(4, 'hp', 1, 0, 345, 422, 0),
(4, 'ac', 1, 0, 61, 81, 0),
(4, 'mana', 1, 0, 180, 220, 0),
(4, 'endurance', 1, 0, 145, 175, 0),
(4, 'heroic_str', 1, 0, 16, 21, 0),
(4, 'heroic_sta', 1, 0, 17, 23, 0),
(4, 'heroic_dex', 1, 0, 10, 14, 0),
(4, 'heroic_agi', 1, 0, 10, 14, 0),
(4, 'heroic_int', 1, 0, 5, 10, 0),
(4, 'heroic_wis', 1, 0, 8, 14, 0),
(4, 'skill_1hs_pct', 0, 100, 2, 9, 0),
(4, 'skill_2hs_pct', 0, 100, 2, 9, 0),
(4, 'skill_pierce_pct', 0, 100, 2, 9, 0),
(4, 'skill_archery_pct', 0, 250, 3, 12, 0),
(4, 'skill_defense_pct', 0, 100, 2, 6, 0),
(4, 'skill_parry_pct', 0, 100, 2, 6, 0),
(4, 'skill_riposte_pct', 0, 100, 2, 6, 0),
(4, 'skill_dodge_pct', 0, 100, 2, 6, 0),
(4, 'spell_fire_pct', 0, 100, 1, 4, 0),
(4, 'spell_cold_pct', 0, 100, 1, 4, 0),
-- Shadowknight
(5, 'hp', 1, 0, 345, 422, 0),
(5, 'ac', 1, 0, 64, 85, 0),
(5, 'mana', 1, 0, 180, 220, 0),
(5, 'endurance', 1, 0, 145, 175, 0),
(5, 'heroic_str', 1, 0, 16, 21, 0),
(5, 'heroic_sta', 1, 0, 17, 23, 0),
(5, 'heroic_dex', 1, 0, 10, 14, 0),
(5, 'heroic_agi', 1, 0, 10, 14, 0),
(5, 'heroic_int', 1, 0, 8, 14, 0),
(5, 'heroic_wis', 1, 0, 5, 10, 0),
(5, 'skill_1hs_pct', 0, 100, 2, 9, 0),
(5, 'skill_2hs_pct', 0, 100, 2, 9, 0),
(5, 'skill_1hb_pct', 0, 100, 2, 9, 0),
(5, 'skill_2hb_pct', 0, 100, 2, 9, 0),
(5, 'skill_defense_pct', 0, 100, 2, 6, 0),
(5, 'skill_parry_pct', 0, 100, 2, 6, 0),
(5, 'skill_riposte_pct', 0, 100, 2, 6, 0),
(5, 'bash_skill_pct', 0, 100, 2, 8, 0),
(5, 'bash_dmg_pct', 0, 100, 4, 14, 0),
(5, 'spell_disease_pct', 0, 100, 1, 4, 0),
(5, 'spell_poison_pct', 0, 100, 1, 4, 0),
(5, 'spell_magic_pct', 0, 100, 1, 4, 0),
-- Druid
(6, 'hp', 1, 0, 300, 370, 0),
(6, 'ac', 1, 0, 62, 84, 0),
(6, 'mana', 1, 0, 360, 440, 0),
(6, 'heroic_str', 1, 0, 5, 10, 0),
(6, 'heroic_sta', 1, 0, 11, 17, 0),
(6, 'heroic_dex', 1, 0, 5, 10, 0),
(6, 'heroic_agi', 1, 0, 5, 10, 0),
(6, 'heroic_int', 1, 0, 5, 10, 0),
(6, 'heroic_wis', 1, 0, 19, 25, 0),
(6, 'skill_1hb_pct', 0, 100, 3, 12, 0),
(6, 'skill_defense_pct', 0, 100, 2, 8, 0),
(6, 'spell_fire_pct', 0, 150, 3, 15, 0),
(6, 'spell_cold_pct', 0, 150, 3, 15, 0),
(6, 'spell_magic_pct', 0, 100, 3, 15, 0),
(6, 'heal_amount_pct', 0, 150, 3, 15, 0),
-- Monk
(7, 'hp', 1, 0, 360, 440, 0),
(7, 'ac', 1, 0, 62, 84, 0),
(7, 'endurance', 1, 0, 290, 350, 0),
(7, 'heroic_str', 1, 0, 19, 25, 0),
(7, 'heroic_sta', 1, 0, 19, 25, 0),
(7, 'heroic_dex', 1, 0, 12, 15, 0),
(7, 'heroic_agi', 1, 0, 12, 15, 0),
(7, 'heroic_int', 1, 0, 5, 10, 0),
(7, 'heroic_wis', 1, 0, 5, 10, 0),
(7, 'skill_h2h_pct', 0, 250, 3, 12, 0),
(7, 'skill_1hb_pct', 0, 100, 3, 12, 0),
(7, 'skill_2hb_pct', 0, 100, 3, 12, 0),
(7, 'skill_defense_pct', 0, 100, 2, 8, 0),
(7, 'skill_block_pct', 0, 100, 2, 8, 0),
(7, 'skill_dodge_pct', 0, 100, 2, 8, 0),
(7, 'skill_riposte_pct', 0, 100, 2, 8, 0),
(7, 'flying_kick_skill_pct', 0, 150, 3, 10, 0),
(7, 'flying_kick_dmg_pct', 0, 200, 5, 18, 0),
-- Bard
(8, 'hp', 1, 0, 345, 422, 0),
(8, 'ac', 1, 0, 57, 76, 0),
(8, 'mana', 1, 0, 180, 220, 0),
(8, 'endurance', 1, 0, 145, 175, 0),
(8, 'heroic_str', 1, 0, 16, 21, 0),
(8, 'heroic_sta', 1, 0, 17, 23, 0),
(8, 'heroic_dex', 1, 0, 10, 14, 0),
(8, 'heroic_agi', 1, 0, 10, 14, 0),
(8, 'heroic_int', 1, 0, 8, 14, 0),
(8, 'heroic_wis', 1, 0, 5, 10, 0),
(8, 'skill_1hs_pct', 0, 100, 2, 9, 0),
(8, 'skill_pierce_pct', 0, 100, 2, 9, 0),
(8, 'skill_1hb_pct', 0, 100, 2, 9, 0),
(8, 'skill_defense_pct', 0, 100, 2, 6, 0),
(8, 'skill_dodge_pct', 0, 100, 2, 6, 0),
(8, 'skill_parry_pct', 0, 100, 2, 6, 0),
(8, 'skill_riposte_pct', 0, 100, 2, 6, 0),
(8, 'bard_stringed_pct', 0, 150, 3, 15, 0),
(8, 'bard_wind_pct', 0, 150, 3, 15, 0),
(8, 'bard_brass_pct', 0, 150, 3, 15, 0),
(8, 'bard_percussion_pct', 0, 150, 3, 15, 0),
(8, 'bard_singing_pct', 0, 150, 3, 15, 0),
-- Rogue
(9, 'hp', 1, 0, 360, 440, 0),
(9, 'ac', 1, 0, 70, 92, 0),
(9, 'endurance', 1, 0, 290, 350, 0),
(9, 'heroic_str', 1, 0, 19, 25, 0),
(9, 'heroic_sta', 1, 0, 19, 25, 0),
(9, 'heroic_dex', 1, 0, 12, 15, 0),
(9, 'heroic_agi', 1, 0, 12, 15, 0),
(9, 'heroic_int', 1, 0, 5, 10, 0),
(9, 'heroic_wis', 1, 0, 5, 10, 0),
(9, 'skill_pierce_pct', 0, 250, 3, 12, 0),
(9, 'skill_1hs_pct', 0, 100, 3, 12, 0),
(9, 'skill_1hb_pct', 0, 100, 3, 12, 0),
(9, 'skill_defense_pct', 0, 100, 2, 8, 0),
(9, 'skill_dodge_pct', 0, 100, 2, 8, 0),
(9, 'skill_parry_pct', 0, 100, 2, 8, 0),
(9, 'skill_riposte_pct', 0, 100, 2, 8, 0),
(9, 'backstab_skill_pct', 0, 150, 3, 10, 0),
(9, 'backstab_dmg_pct', 0, 250, 5, 18, 0),
-- Shaman
(10, 'hp', 1, 0, 300, 370, 0),
(10, 'ac', 1, 0, 70, 92, 0),
(10, 'mana', 1, 0, 360, 440, 0),
(10, 'heroic_str', 1, 0, 11, 17, 0),
(10, 'heroic_sta', 1, 0, 11, 17, 0),
(10, 'heroic_dex', 1, 0, 5, 10, 0),
(10, 'heroic_agi', 1, 0, 5, 10, 0),
(10, 'heroic_int', 1, 0, 5, 10, 0),
(10, 'heroic_wis', 1, 0, 19, 25, 0),
(10, 'skill_1hb_pct', 0, 100, 3, 12, 0),
(10, 'skill_2hb_pct', 0, 100, 3, 12, 0),
(10, 'skill_defense_pct', 0, 100, 2, 8, 0),
(10, 'spell_cold_pct', 0, 100, 3, 15, 0),
(10, 'spell_poison_pct', 0, 100, 3, 15, 0),
(10, 'spell_disease_pct', 0, 100, 3, 15, 0),
(10, 'heal_amount_pct', 0, 150, 3, 15, 0),
-- Necromancer
(11, 'hp', 1, 0, 300, 370, 0),
(11, 'ac', 1, 0, 45, 64, 0),
(11, 'mana', 1, 0, 360, 440, 0),
(11, 'heroic_str', 1, 0, 5, 10, 0),
(11, 'heroic_sta', 1, 0, 11, 17, 0),
(11, 'heroic_dex', 1, 0, 5, 10, 0),
(11, 'heroic_agi', 1, 0, 5, 10, 0),
(11, 'heroic_int', 1, 0, 19, 25, 0),
(11, 'heroic_wis', 1, 0, 5, 10, 0),
(11, 'spell_fire_pct', 0, 100, 3, 15, 0),
(11, 'spell_poison_pct', 0, 150, 3, 15, 0),
(11, 'spell_disease_pct', 0, 150, 3, 15, 0),
(11, 'spell_magic_pct', 0, 100, 3, 15, 0),
-- Wizard
(12, 'hp', 1, 0, 300, 370, 0),
(12, 'ac', 1, 0, 45, 64, 0),
(12, 'mana', 1, 0, 360, 440, 0),
(12, 'heroic_str', 1, 0, 5, 10, 0),
(12, 'heroic_sta', 1, 0, 11, 17, 0),
(12, 'heroic_dex', 1, 0, 5, 10, 0),
(12, 'heroic_agi', 1, 0, 5, 10, 0),
(12, 'heroic_int', 1, 0, 19, 25, 0),
(12, 'heroic_wis', 1, 0, 5, 10, 0),
(12, 'spell_fire_pct', 0, 150, 3, 15, 0),
(12, 'spell_cold_pct', 0, 150, 3, 15, 0),
(12, 'spell_magic_pct', 0, 150, 3, 15, 0),
-- Magician
(13, 'hp', 1, 0, 300, 370, 0),
(13, 'ac', 1, 0, 45, 64, 0),
(13, 'mana', 1, 0, 360, 440, 0),
(13, 'heroic_str', 1, 0, 5, 10, 0),
(13, 'heroic_sta', 1, 0, 11, 17, 0),
(13, 'heroic_dex', 1, 0, 5, 10, 0),
(13, 'heroic_agi', 1, 0, 5, 10, 0),
(13, 'heroic_int', 1, 0, 19, 25, 0),
(13, 'heroic_wis', 1, 0, 5, 10, 0),
(13, 'spell_fire_pct', 0, 200, 3, 15, 0),
(13, 'spell_magic_pct', 0, 150, 3, 15, 0),
(13, 'spell_cold_pct', 0, 100, 3, 15, 0),
-- Enchanter
(14, 'hp', 1, 0, 300, 370, 0),
(14, 'ac', 1, 0, 45, 64, 0),
(14, 'mana', 1, 0, 360, 440, 0),
(14, 'heroic_str', 1, 0, 5, 10, 0),
(14, 'heroic_sta', 1, 0, 11, 17, 0),
(14, 'heroic_dex', 1, 0, 5, 10, 0),
(14, 'heroic_agi', 1, 0, 5, 10, 0),
(14, 'heroic_int', 1, 0, 19, 25, 0),
(14, 'heroic_wis', 1, 0, 5, 10, 0),
(14, 'spell_magic_pct', 0, 200, 3, 15, 0),
(14, 'spell_poison_pct', 0, 100, 3, 15, 0),
(14, 'spell_cold_pct', 0, 100, 3, 15, 0),
-- Beastlord
(15, 'hp', 1, 0, 345, 422, 0),
(15, 'ac', 1, 0, 63, 83, 0),
(15, 'mana', 1, 0, 180, 220, 0),
(15, 'endurance', 1, 0, 145, 175, 0),
(15, 'heroic_str', 1, 0, 17, 23, 0),
(15, 'heroic_sta', 1, 0, 17, 23, 0),
(15, 'heroic_dex', 1, 0, 10, 14, 0),
(15, 'heroic_agi', 1, 0, 10, 14, 0),
(15, 'heroic_int', 1, 0, 5, 10, 0),
(15, 'heroic_wis', 1, 0, 8, 14, 0),
(15, 'skill_h2h_pct', 0, 200, 2, 9, 0),
(15, 'skill_1hb_pct', 0, 100, 2, 9, 0),
(15, 'skill_2hb_pct', 0, 100, 2, 9, 0),
(15, 'skill_pierce_pct', 0, 100, 2, 9, 0),
(15, 'skill_block_pct', 0, 100, 2, 6, 0),
(15, 'skill_dodge_pct', 0, 100, 2, 6, 0),
(15, 'skill_parry_pct', 0, 100, 2, 6, 0),
(15, 'skill_riposte_pct', 0, 100, 2, 6, 0),
(15, 'spell_cold_pct', 0, 100, 1, 4, 0),
(15, 'spell_poison_pct', 0, 100, 1, 4, 0),
(15, 'heal_amount_pct', 0, 100, 1, 4, 0),
-- Berserker
(16, 'hp', 1, 0, 360, 440, 0),
(16, 'ac', 1, 0, 70, 92, 0),
(16, 'endurance', 1, 0, 290, 350, 0),
(16, 'heroic_str', 1, 0, 19, 25, 0),
(16, 'heroic_sta', 1, 0, 19, 25, 0),
(16, 'heroic_dex', 1, 0, 12, 15, 0),
(16, 'heroic_agi', 1, 0, 11, 15, 0),
(16, 'heroic_int', 1, 0, 5, 10, 0),
(16, 'heroic_wis', 1, 0, 5, 10, 0),
(16, 'skill_2hs_pct', 0, 100, 3, 12, 0),
(16, 'skill_2hb_pct', 0, 100, 3, 12, 0),
(16, 'skill_defense_pct', 0, 100, 2, 8, 0),
(16, 'skill_parry_pct', 0, 100, 2, 8, 0),
(16, 'skill_riposte_pct', 0, 100, 2, 8, 0),
(16, 'skill_dodge_pct', 0, 100, 2, 8, 0),
(16, 'frenzy_skill_pct', 0, 150, 3, 10, 0),
(16, 'frenzy_dmg_pct', 0, 250, 5, 18, 0);
)",
		.content_schema_update = true,
	},
	ManifestEntry{
		.version = 2,
		.description = "2026_10_21_dynamic_affix_suffixes",
		.check = "SHOW TABLES LIKE 'dynamic_affix_suffixes'",
		.condition = "empty",
		.match = "",
		.sql = R"(
CREATE TABLE IF NOT EXISTS `dynamic_affix_suffixes` (
  `affix` VARCHAR(32) NOT NULL,
  `suffix` VARCHAR(48) NOT NULL,
  PRIMARY KEY (`affix`)
);

INSERT IGNORE INTO `dynamic_affix_suffixes` (`affix`, `suffix`) VALUES
('skill_1hs_pct', 'of Slashing'),
('skill_2hs_pct', 'of Cleaving'),
('skill_1hb_pct', 'of Crushing'),
('skill_2hb_pct', 'of Smashing'),
('skill_pierce_pct', 'of Piercing'),
('skill_h2h_pct', 'of the Fist'),
('skill_archery_pct', 'of the Hawk'),
('skill_defense_pct', 'of Defense'),
('skill_parry_pct', 'of Parrying'),
('skill_riposte_pct', 'of Retaliation'),
('skill_dodge_pct', 'of Evasion'),
('skill_block_pct', 'of Blocking'),
('flying_kick_skill_pct', 'of the Crane'),
('flying_kick_dmg_pct', 'of the Tiger'),
('backstab_skill_pct', 'of Shadows'),
('backstab_dmg_pct', 'of the Assassin'),
('frenzy_skill_pct', 'of Fury'),
('frenzy_dmg_pct', 'of the Berserker'),
('bash_skill_pct', 'of the Ram'),
('bash_dmg_pct', 'of Battering'),
('spell_fire_pct', 'of Flames'),
('spell_cold_pct', 'of Frost'),
('spell_magic_pct', 'of Sorcery'),
('spell_poison_pct', 'of Venom'),
('spell_disease_pct', 'of Plague'),
('heal_amount_pct', 'of Mending'),
('bard_stringed_pct', 'of Strings'),
('bard_wind_pct', 'of the Wind'),
('bard_brass_pct', 'of Brass'),
('bard_percussion_pct', 'of the Drum'),
('bard_singing_pct', 'of Song');
)",
		.content_schema_update = true,
	},
	ManifestEntry{
		.version = 3,
		.description = "2026_10_21_dynamic_items_stats_resists_advanced",
		.check = "SELECT 1 FROM dynamic_affix_pool WHERE affix = 'str' LIMIT 1",
		.condition = "empty",
		.match = "",
		.sql = R"(
INSERT IGNORE INTO `dynamic_affix_pool` (`class_id`, `affix`, `guaranteed`, `weight`, `min_value`, `max_value`, `slot_mask`) VALUES
-- Warrior
(1, 'str', 1, 0, 14, 26, 0),
(1, 'sta', 1, 0, 14, 26, 0),
(1, 'dex', 1, 0, 9, 16, 0),
(1, 'agi', 1, 0, 14, 26, 0),
(1, 'int', 1, 0, 3, 8, 0),
(1, 'wis', 1, 0, 3, 8, 0),
(1, 'cha', 1, 0, 3, 8, 0),
(1, 'mr', 1, 0, 15, 25, 0),
(1, 'fr', 1, 0, 15, 25, 0),
(1, 'cr', 1, 0, 15, 25, 0),
(1, 'dr', 1, 0, 15, 25, 0),
(1, 'pr', 1, 0, 15, 25, 0),
(1, 'corruption', 1, 0, 15, 25, 0),
(1, 'heroic_mr', 0, 20, 2, 10, 0),
(1, 'heroic_fr', 0, 20, 2, 10, 0),
(1, 'heroic_cr', 0, 20, 2, 10, 0),
(1, 'heroic_dr', 0, 20, 2, 10, 0),
(1, 'heroic_pr', 0, 20, 2, 10, 0),
(1, 'heroic_corruption', 0, 20, 2, 10, 0),
(1, 'spelldmg', 1, 0, 1, 5, 0),
(1, 'healamt', 1, 0, 1, 5, 0),
(1, 'attack', 1, 0, 12, 30, 0),
(1, 'hp_regen', 1, 0, 0, 3, 0),
(1, 'end_regen', 1, 0, 0, 3, 0),
(1, 'combat_effects', 1, 0, 0, 3, 0),
(1, 'spell_shield', 1, 0, 0, 3, 0),
(1, 'shielding', 1, 0, 0, 3, 0),
(1, 'damage_shield', 1, 0, 0, 3, 0),
(1, 'dot_shielding', 1, 0, 0, 3, 0),
(1, 'ds_mitigation', 1, 0, 0, 3, 0),
(1, 'avoidance', 1, 0, 0, 3, 0),
(1, 'accuracy', 1, 0, 0, 3, 0),
(1, 'stun_resist', 1, 0, 0, 3, 0),
(1, 'strike_through', 1, 0, 0, 3, 0),
(1, 'spell_crit_chance_pct', 1, 0, 0, 2, 0),
(1, 'spell_crit_dmg_pct', 1, 0, 0, 3, 0),
(1, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(1, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(1, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(1, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(1, 'melee_crit_chance_pct', 1, 0, 0, 6, 0),
(1, 'melee_crit_dmg_pct', 1, 0, 0, 10, 0),
(1, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Cleric
(2, 'str', 1, 0, 9, 16, 0),
(2, 'sta', 1, 0, 9, 16, 0),
(2, 'dex', 1, 0, 3, 8, 0),
(2, 'agi', 1, 0, 3, 8, 0),
(2, 'int', 1, 0, 3, 8, 0),
(2, 'wis', 1, 0, 14, 26, 0),
(2, 'cha', 1, 0, 3, 8, 0),
(2, 'mr', 1, 0, 15, 25, 0),
(2, 'fr', 1, 0, 15, 25, 0),
(2, 'cr', 1, 0, 15, 25, 0),
(2, 'dr', 1, 0, 15, 25, 0),
(2, 'pr', 1, 0, 15, 25, 0),
(2, 'corruption', 1, 0, 15, 25, 0),
(2, 'heroic_mr', 0, 20, 2, 10, 0),
(2, 'heroic_fr', 0, 20, 2, 10, 0),
(2, 'heroic_cr', 0, 20, 2, 10, 0),
(2, 'heroic_dr', 0, 20, 2, 10, 0),
(2, 'heroic_pr', 0, 20, 2, 10, 0),
(2, 'heroic_corruption', 0, 20, 2, 10, 0),
(2, 'spelldmg', 1, 0, 5, 15, 0),
(2, 'healamt', 1, 0, 10, 30, 0),
(2, 'attack', 1, 0, 1, 5, 0),
(2, 'hp_regen', 1, 0, 0, 3, 0),
(2, 'mana_regen', 1, 0, 0, 3, 0),
(2, 'spell_shield', 1, 0, 0, 3, 0),
(2, 'dot_shielding', 1, 0, 0, 3, 0),
(2, 'avoidance', 1, 0, 0, 3, 0),
(2, 'stun_resist', 1, 0, 0, 3, 0),
(2, 'spell_crit_chance_pct', 1, 0, 0, 2, 0),
(2, 'spell_crit_dmg_pct', 1, 0, 0, 3, 0),
(2, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(2, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(2, 'heal_crit_chance_pct', 1, 0, 0, 6, 0),
(2, 'hot_crit_chance_pct', 1, 0, 0, 6, 0),
(2, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(2, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(2, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Paladin
(3, 'str', 1, 0, 13, 24, 0),
(3, 'sta', 1, 0, 13, 24, 0),
(3, 'dex', 1, 0, 8, 14, 0),
(3, 'agi', 1, 0, 11, 22, 0),
(3, 'int', 1, 0, 3, 8, 0),
(3, 'wis', 1, 0, 6, 13, 0),
(3, 'cha', 1, 0, 3, 8, 0),
(3, 'mr', 1, 0, 15, 25, 0),
(3, 'fr', 1, 0, 15, 25, 0),
(3, 'cr', 1, 0, 15, 25, 0),
(3, 'dr', 1, 0, 15, 25, 0),
(3, 'pr', 1, 0, 15, 25, 0),
(3, 'corruption', 1, 0, 15, 25, 0),
(3, 'heroic_mr', 0, 20, 2, 10, 0),
(3, 'heroic_fr', 0, 20, 2, 10, 0),
(3, 'heroic_cr', 0, 20, 2, 10, 0),
(3, 'heroic_dr', 0, 20, 2, 10, 0),
(3, 'heroic_pr', 0, 20, 2, 10, 0),
(3, 'heroic_corruption', 0, 20, 2, 10, 0),
(3, 'spelldmg', 1, 0, 2, 8, 0),
(3, 'healamt', 1, 0, 3, 11, 0),
(3, 'attack', 1, 0, 9, 24, 0),
(3, 'hp_regen', 1, 0, 0, 3, 0),
(3, 'mana_regen', 1, 0, 0, 1, 0),
(3, 'end_regen', 1, 0, 0, 2, 0),
(3, 'combat_effects', 1, 0, 0, 2, 0),
(3, 'spell_shield', 1, 0, 0, 3, 0),
(3, 'shielding', 1, 0, 0, 2, 0),
(3, 'damage_shield', 1, 0, 0, 2, 0),
(3, 'dot_shielding', 1, 0, 0, 3, 0),
(3, 'ds_mitigation', 1, 0, 0, 2, 0),
(3, 'avoidance', 1, 0, 0, 3, 0),
(3, 'accuracy', 1, 0, 0, 2, 0),
(3, 'stun_resist', 1, 0, 0, 3, 0),
(3, 'strike_through', 1, 0, 0, 2, 0),
(3, 'spell_crit_chance_pct', 1, 0, 0, 2, 0),
(3, 'spell_crit_dmg_pct', 1, 0, 0, 3, 0),
(3, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(3, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(3, 'heal_crit_chance_pct', 1, 0, 0, 3, 0),
(3, 'hot_crit_chance_pct', 1, 0, 0, 3, 0),
(3, 'melee_crit_chance_pct', 1, 0, 0, 5, 0),
(3, 'melee_crit_dmg_pct', 1, 0, 0, 8, 0),
(3, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Ranger
(4, 'str', 1, 0, 8, 14, 0),
(4, 'sta', 1, 0, 9, 16, 0),
(4, 'dex', 1, 0, 13, 23, 0),
(4, 'agi', 1, 0, 11, 22, 0),
(4, 'int', 1, 0, 3, 8, 0),
(4, 'wis', 1, 0, 6, 13, 0),
(4, 'cha', 1, 0, 3, 8, 0),
(4, 'mr', 1, 0, 15, 25, 0),
(4, 'fr', 1, 0, 15, 25, 0),
(4, 'cr', 1, 0, 15, 25, 0),
(4, 'dr', 1, 0, 15, 25, 0),
(4, 'pr', 1, 0, 15, 25, 0),
(4, 'corruption', 1, 0, 15, 25, 0),
(4, 'heroic_mr', 0, 20, 2, 10, 0),
(4, 'heroic_fr', 0, 20, 2, 10, 0),
(4, 'heroic_cr', 0, 20, 2, 10, 0),
(4, 'heroic_dr', 0, 20, 2, 10, 0),
(4, 'heroic_pr', 0, 20, 2, 10, 0),
(4, 'heroic_corruption', 0, 20, 2, 10, 0),
(4, 'spelldmg', 1, 0, 2, 8, 0),
(4, 'healamt', 1, 0, 3, 11, 0),
(4, 'attack', 1, 0, 9, 24, 0),
(4, 'hp_regen', 1, 0, 0, 3, 0),
(4, 'mana_regen', 1, 0, 0, 1, 0),
(4, 'end_regen', 1, 0, 0, 2, 0),
(4, 'combat_effects', 1, 0, 0, 2, 0);
)"
			R"(
INSERT IGNORE INTO `dynamic_affix_pool` (`class_id`, `affix`, `guaranteed`, `weight`, `min_value`, `max_value`, `slot_mask`) VALUES
(4, 'spell_shield', 1, 0, 0, 3, 0),
(4, 'shielding', 1, 0, 0, 2, 0),
(4, 'dot_shielding', 1, 0, 0, 3, 0),
(4, 'ds_mitigation', 1, 0, 0, 2, 0),
(4, 'avoidance', 1, 0, 0, 3, 0),
(4, 'accuracy', 1, 0, 0, 2, 0),
(4, 'stun_resist', 1, 0, 0, 3, 0),
(4, 'strike_through', 1, 0, 0, 2, 0),
(4, 'spell_crit_chance_pct', 1, 0, 0, 3, 0),
(4, 'spell_crit_dmg_pct', 1, 0, 0, 4, 0),
(4, 'dot_crit_chance_pct', 1, 0, 0, 3, 0),
(4, 'dot_crit_dmg_pct', 1, 0, 0, 5, 0),
(4, 'heal_crit_chance_pct', 1, 0, 0, 3, 0),
(4, 'hot_crit_chance_pct', 1, 0, 0, 3, 0),
(4, 'melee_crit_chance_pct', 1, 0, 0, 5, 0),
(4, 'melee_crit_dmg_pct', 1, 0, 0, 8, 0),
(4, 'archery_crit_chance_pct', 1, 0, 0, 6, 0),
-- Shadowknight
(5, 'str', 1, 0, 11, 22, 0),
(5, 'sta', 1, 0, 13, 24, 0),
(5, 'dex', 1, 0, 8, 14, 0),
(5, 'agi', 1, 0, 11, 22, 0),
(5, 'int', 1, 0, 6, 13, 0),
(5, 'wis', 1, 0, 3, 8, 0),
(5, 'cha', 1, 0, 3, 8, 0),
(5, 'mr', 1, 0, 15, 25, 0),
(5, 'fr', 1, 0, 15, 25, 0),
(5, 'cr', 1, 0, 15, 25, 0),
(5, 'dr', 1, 0, 15, 25, 0),
(5, 'pr', 1, 0, 15, 25, 0),
(5, 'corruption', 1, 0, 15, 25, 0),
(5, 'heroic_mr', 0, 20, 2, 10, 0),
(5, 'heroic_fr', 0, 20, 2, 10, 0),
(5, 'heroic_cr', 0, 20, 2, 10, 0),
(5, 'heroic_dr', 0, 20, 2, 10, 0),
(5, 'heroic_pr', 0, 20, 2, 10, 0),
(5, 'heroic_corruption', 0, 20, 2, 10, 0),
(5, 'spelldmg', 1, 0, 3, 11, 0),
(5, 'healamt', 1, 0, 1, 5, 0),
(5, 'attack', 1, 0, 9, 24, 0),
(5, 'hp_regen', 1, 0, 0, 3, 0),
(5, 'mana_regen', 1, 0, 0, 1, 0),
(5, 'end_regen', 1, 0, 0, 2, 0),
(5, 'combat_effects', 1, 0, 0, 2, 0),
(5, 'spell_shield', 1, 0, 0, 3, 0),
(5, 'shielding', 1, 0, 0, 2, 0),
(5, 'damage_shield', 1, 0, 0, 2, 0),
(5, 'dot_shielding', 1, 0, 0, 3, 0),
(5, 'ds_mitigation', 1, 0, 0, 2, 0),
(5, 'avoidance', 1, 0, 0, 3, 0),
(5, 'accuracy', 1, 0, 0, 2, 0),
(5, 'stun_resist', 1, 0, 0, 3, 0),
(5, 'strike_through', 1, 0, 0, 2, 0),
(5, 'spell_crit_chance_pct', 1, 0, 0, 3, 0),
(5, 'spell_crit_dmg_pct', 1, 0, 0, 4, 0),
(5, 'dot_crit_chance_pct', 1, 0, 0, 3, 0),
(5, 'dot_crit_dmg_pct', 1, 0, 0, 5, 0),
(5, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(5, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(5, 'melee_crit_chance_pct', 1, 0, 0, 5, 0),
(5, 'melee_crit_dmg_pct', 1, 0, 0, 8, 0),
(5, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Druid
(6, 'str', 1, 0, 3, 8, 0),
(6, 'sta', 1, 0, 9, 16, 0),
(6, 'dex', 1, 0, 3, 8, 0),
(6, 'agi', 1, 0, 3, 8, 0),
(6, 'int', 1, 0, 3, 8, 0),
(6, 'wis', 1, 0, 14, 26, 0),
(6, 'cha', 1, 0, 3, 8, 0),
(6, 'mr', 1, 0, 15, 25, 0),
(6, 'fr', 1, 0, 15, 25, 0),
(6, 'cr', 1, 0, 15, 25, 0),
(6, 'dr', 1, 0, 15, 25, 0),
(6, 'pr', 1, 0, 15, 25, 0),
(6, 'corruption', 1, 0, 15, 25, 0),
(6, 'heroic_mr', 0, 20, 2, 10, 0),
(6, 'heroic_fr', 0, 20, 2, 10, 0),
(6, 'heroic_cr', 0, 20, 2, 10, 0),
(6, 'heroic_dr', 0, 20, 2, 10, 0),
(6, 'heroic_pr', 0, 20, 2, 10, 0),
(6, 'heroic_corruption', 0, 20, 2, 10, 0),
(6, 'spelldmg', 1, 0, 5, 15, 0),
(6, 'healamt', 1, 0, 10, 30, 0),
(6, 'attack', 1, 0, 1, 5, 0),
(6, 'hp_regen', 1, 0, 0, 3, 0),
(6, 'mana_regen', 1, 0, 0, 3, 0),
(6, 'spell_shield', 1, 0, 0, 3, 0),
(6, 'dot_shielding', 1, 0, 0, 3, 0),
(6, 'avoidance', 1, 0, 0, 3, 0),
(6, 'stun_resist', 1, 0, 0, 3, 0),
(6, 'spell_crit_chance_pct', 1, 0, 0, 4, 0),
(6, 'spell_crit_dmg_pct', 1, 0, 0, 6, 0),
(6, 'dot_crit_chance_pct', 1, 0, 0, 6, 0),
(6, 'dot_crit_dmg_pct', 1, 0, 0, 10, 0),
(6, 'heal_crit_chance_pct', 1, 0, 0, 6, 0),
(6, 'hot_crit_chance_pct', 1, 0, 0, 6, 0),
(6, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(6, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(6, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Monk
(7, 'str', 1, 0, 14, 26, 0),
(7, 'sta', 1, 0, 9, 16, 0),
(7, 'dex', 1, 0, 16, 28, 0),
(7, 'agi', 1, 0, 14, 26, 0),
(7, 'int', 1, 0, 3, 8, 0),
(7, 'wis', 1, 0, 3, 8, 0),
(7, 'cha', 1, 0, 3, 8, 0),
(7, 'mr', 1, 0, 15, 25, 0),
(7, 'fr', 1, 0, 15, 25, 0),
(7, 'cr', 1, 0, 15, 25, 0),
(7, 'dr', 1, 0, 15, 25, 0),
(7, 'pr', 1, 0, 15, 25, 0),
(7, 'corruption', 1, 0, 15, 25, 0),
(7, 'heroic_mr', 0, 20, 2, 10, 0),
(7, 'heroic_fr', 0, 20, 2, 10, 0),
(7, 'heroic_cr', 0, 20, 2, 10, 0),
(7, 'heroic_dr', 0, 20, 2, 10, 0),
(7, 'heroic_pr', 0, 20, 2, 10, 0),
(7, 'heroic_corruption', 0, 20, 2, 10, 0),
(7, 'spelldmg', 1, 0, 1, 5, 0),
(7, 'healamt', 1, 0, 1, 5, 0),
(7, 'attack', 1, 0, 12, 30, 0),
(7, 'hp_regen', 1, 0, 0, 3, 0),
(7, 'end_regen', 1, 0, 0, 3, 0),
(7, 'combat_effects', 1, 0, 0, 3, 0),
(7, 'spell_shield', 1, 0, 0, 3, 0),
(7, 'shielding', 1, 0, 0, 3, 0),
(7, 'dot_shielding', 1, 0, 0, 3, 0),
(7, 'ds_mitigation', 1, 0, 0, 3, 0),
(7, 'avoidance', 1, 0, 0, 3, 0),
(7, 'accuracy', 1, 0, 0, 3, 0),
(7, 'stun_resist', 1, 0, 0, 3, 0),
(7, 'strike_through', 1, 0, 0, 3, 0),
(7, 'spell_crit_chance_pct', 1, 0, 0, 2, 0),
(7, 'spell_crit_dmg_pct', 1, 0, 0, 3, 0),
(7, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(7, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(7, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(7, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(7, 'melee_crit_chance_pct', 1, 0, 0, 6, 0),
(7, 'melee_crit_dmg_pct', 1, 0, 0, 10, 0),
(7, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Bard
(8, 'str', 1, 0, 8, 14, 0),
(8, 'sta', 1, 0, 9, 16, 0),
(8, 'dex', 1, 0, 13, 23, 0),
(8, 'agi', 1, 0, 11, 22, 0),
(8, 'int', 1, 0, 6, 13, 0),
(8, 'wis', 1, 0, 3, 8, 0),
(8, 'cha', 1, 0, 14, 26, 0),
(8, 'mr', 1, 0, 15, 25, 0),
(8, 'fr', 1, 0, 15, 25, 0),
(8, 'cr', 1, 0, 15, 25, 0);
)"
			R"(
INSERT IGNORE INTO `dynamic_affix_pool` (`class_id`, `affix`, `guaranteed`, `weight`, `min_value`, `max_value`, `slot_mask`) VALUES
(8, 'dr', 1, 0, 15, 25, 0),
(8, 'pr', 1, 0, 15, 25, 0),
(8, 'corruption', 1, 0, 15, 25, 0),
(8, 'heroic_mr', 0, 20, 2, 10, 0),
(8, 'heroic_fr', 0, 20, 2, 10, 0),
(8, 'heroic_cr', 0, 20, 2, 10, 0),
(8, 'heroic_dr', 0, 20, 2, 10, 0),
(8, 'heroic_pr', 0, 20, 2, 10, 0),
(8, 'heroic_corruption', 0, 20, 2, 10, 0),
(8, 'spelldmg', 1, 0, 3, 11, 0),
(8, 'healamt', 1, 0, 1, 5, 0),
(8, 'attack', 1, 0, 9, 24, 0),
(8, 'hp_regen', 1, 0, 0, 3, 0),
(8, 'mana_regen', 1, 0, 0, 1, 0),
(8, 'end_regen', 1, 0, 0, 2, 0),
(8, 'combat_effects', 1, 0, 0, 2, 0),
(8, 'spell_shield', 1, 0, 0, 3, 0),
(8, 'shielding', 1, 0, 0, 2, 0),
(8, 'dot_shielding', 1, 0, 0, 3, 0),
(8, 'ds_mitigation', 1, 0, 0, 2, 0),
(8, 'avoidance', 1, 0, 0, 3, 0),
(8, 'accuracy', 1, 0, 0, 2, 0),
(8, 'stun_resist', 1, 0, 0, 3, 0),
(8, 'strike_through', 1, 0, 0, 2, 0),
(8, 'spell_crit_chance_pct', 1, 0, 0, 3, 0),
(8, 'spell_crit_dmg_pct', 1, 0, 0, 4, 0),
(8, 'dot_crit_chance_pct', 1, 0, 0, 3, 0),
(8, 'dot_crit_dmg_pct', 1, 0, 0, 4, 0),
(8, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(8, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(8, 'melee_crit_chance_pct', 1, 0, 0, 5, 0),
(8, 'melee_crit_dmg_pct', 1, 0, 0, 8, 0),
(8, 'archery_crit_chance_pct', 1, 0, 0, 4, 0),
-- Rogue
(9, 'str', 1, 0, 9, 16, 0),
(9, 'sta', 1, 0, 9, 16, 0),
(9, 'dex', 1, 0, 16, 28, 0),
(9, 'agi', 1, 0, 14, 26, 0),
(9, 'int', 1, 0, 3, 8, 0),
(9, 'wis', 1, 0, 3, 8, 0),
(9, 'cha', 1, 0, 3, 8, 0),
(9, 'mr', 1, 0, 15, 25, 0),
(9, 'fr', 1, 0, 15, 25, 0),
(9, 'cr', 1, 0, 15, 25, 0),
(9, 'dr', 1, 0, 15, 25, 0),
(9, 'pr', 1, 0, 15, 25, 0),
(9, 'corruption', 1, 0, 15, 25, 0),
(9, 'heroic_mr', 0, 20, 2, 10, 0),
(9, 'heroic_fr', 0, 20, 2, 10, 0),
(9, 'heroic_cr', 0, 20, 2, 10, 0),
(9, 'heroic_dr', 0, 20, 2, 10, 0),
(9, 'heroic_pr', 0, 20, 2, 10, 0),
(9, 'heroic_corruption', 0, 20, 2, 10, 0),
(9, 'spelldmg', 1, 0, 1, 5, 0),
(9, 'healamt', 1, 0, 1, 5, 0),
(9, 'attack', 1, 0, 12, 30, 0),
(9, 'hp_regen', 1, 0, 0, 3, 0),
(9, 'end_regen', 1, 0, 0, 3, 0),
(9, 'combat_effects', 1, 0, 0, 3, 0),
(9, 'spell_shield', 1, 0, 0, 3, 0),
(9, 'shielding', 1, 0, 0, 3, 0),
(9, 'dot_shielding', 1, 0, 0, 3, 0),
(9, 'ds_mitigation', 1, 0, 0, 3, 0),
(9, 'avoidance', 1, 0, 0, 3, 0),
(9, 'accuracy', 1, 0, 0, 3, 0),
(9, 'stun_resist', 1, 0, 0, 3, 0),
(9, 'strike_through', 1, 0, 0, 3, 0),
(9, 'spell_crit_chance_pct', 1, 0, 0, 2, 0),
(9, 'spell_crit_dmg_pct', 1, 0, 0, 3, 0),
(9, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(9, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(9, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(9, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(9, 'melee_crit_chance_pct', 1, 0, 0, 6, 0),
(9, 'melee_crit_dmg_pct', 1, 0, 0, 10, 0),
(9, 'archery_crit_chance_pct', 1, 0, 0, 4, 0),
-- Shaman
(10, 'str', 1, 0, 9, 16, 0),
(10, 'sta', 1, 0, 9, 16, 0),
(10, 'dex', 1, 0, 3, 8, 0),
(10, 'agi', 1, 0, 3, 8, 0),
(10, 'int', 1, 0, 3, 8, 0),
(10, 'wis', 1, 0, 14, 26, 0),
(10, 'cha', 1, 0, 3, 8, 0),
(10, 'mr', 1, 0, 15, 25, 0),
(10, 'fr', 1, 0, 15, 25, 0),
(10, 'cr', 1, 0, 15, 25, 0),
(10, 'dr', 1, 0, 15, 25, 0),
(10, 'pr', 1, 0, 15, 25, 0),
(10, 'corruption', 1, 0, 15, 25, 0),
(10, 'heroic_mr', 0, 20, 2, 10, 0),
(10, 'heroic_fr', 0, 20, 2, 10, 0),
(10, 'heroic_cr', 0, 20, 2, 10, 0),
(10, 'heroic_dr', 0, 20, 2, 10, 0),
(10, 'heroic_pr', 0, 20, 2, 10, 0),
(10, 'heroic_corruption', 0, 20, 2, 10, 0),
(10, 'spelldmg', 1, 0, 5, 15, 0),
(10, 'healamt', 1, 0, 10, 30, 0),
(10, 'attack', 1, 0, 1, 5, 0),
(10, 'hp_regen', 1, 0, 0, 3, 0),
(10, 'mana_regen', 1, 0, 0, 3, 0),
(10, 'spell_shield', 1, 0, 0, 3, 0),
(10, 'dot_shielding', 1, 0, 0, 3, 0),
(10, 'avoidance', 1, 0, 0, 3, 0),
(10, 'stun_resist', 1, 0, 0, 3, 0),
(10, 'spell_crit_chance_pct', 1, 0, 0, 4, 0),
(10, 'spell_crit_dmg_pct', 1, 0, 0, 6, 0),
(10, 'dot_crit_chance_pct', 1, 0, 0, 4, 0),
(10, 'dot_crit_dmg_pct', 1, 0, 0, 6, 0),
(10, 'heal_crit_chance_pct', 1, 0, 0, 6, 0),
(10, 'hot_crit_chance_pct', 1, 0, 0, 6, 0),
(10, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(10, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(10, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Necromancer
(11, 'str', 1, 0, 3, 8, 0),
(11, 'sta', 1, 0, 9, 16, 0),
(11, 'dex', 1, 0, 3, 8, 0),
(11, 'agi', 1, 0, 3, 8, 0),
(11, 'int', 1, 0, 14, 26, 0),
(11, 'wis', 1, 0, 3, 8, 0),
(11, 'cha', 1, 0, 3, 8, 0),
(11, 'mr', 1, 0, 15, 25, 0),
(11, 'fr', 1, 0, 15, 25, 0),
(11, 'cr', 1, 0, 15, 25, 0),
(11, 'dr', 1, 0, 15, 25, 0),
(11, 'pr', 1, 0, 15, 25, 0),
(11, 'corruption', 1, 0, 15, 25, 0),
(11, 'heroic_mr', 0, 20, 2, 10, 0),
(11, 'heroic_fr', 0, 20, 2, 10, 0),
(11, 'heroic_cr', 0, 20, 2, 10, 0),
(11, 'heroic_dr', 0, 20, 2, 10, 0),
(11, 'heroic_pr', 0, 20, 2, 10, 0),
(11, 'heroic_corruption', 0, 20, 2, 10, 0),
(11, 'spelldmg', 1, 0, 10, 30, 0),
(11, 'healamt', 1, 0, 1, 5, 0),
(11, 'attack', 1, 0, 1, 5, 0),
(11, 'hp_regen', 1, 0, 0, 3, 0),
(11, 'mana_regen', 1, 0, 0, 3, 0),
(11, 'spell_shield', 1, 0, 0, 3, 0),
(11, 'dot_shielding', 1, 0, 0, 3, 0),
(11, 'avoidance', 1, 0, 0, 3, 0),
(11, 'stun_resist', 1, 0, 0, 3, 0),
(11, 'spell_crit_chance_pct', 1, 0, 0, 4, 0),
(11, 'spell_crit_dmg_pct', 1, 0, 0, 6, 0),
(11, 'dot_crit_chance_pct', 1, 0, 0, 6, 0),
(11, 'dot_crit_dmg_pct', 1, 0, 0, 10, 0),
(11, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(11, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(11, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(11, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(11, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Wizard
(12, 'str', 1, 0, 3, 8, 0);
)"
			R"(
INSERT IGNORE INTO `dynamic_affix_pool` (`class_id`, `affix`, `guaranteed`, `weight`, `min_value`, `max_value`, `slot_mask`) VALUES
(12, 'sta', 1, 0, 9, 16, 0),
(12, 'dex', 1, 0, 3, 8, 0),
(12, 'agi', 1, 0, 3, 8, 0),
(12, 'int', 1, 0, 14, 26, 0),
(12, 'wis', 1, 0, 3, 8, 0),
(12, 'cha', 1, 0, 3, 8, 0),
(12, 'mr', 1, 0, 15, 25, 0),
(12, 'fr', 1, 0, 15, 25, 0),
(12, 'cr', 1, 0, 15, 25, 0),
(12, 'dr', 1, 0, 15, 25, 0),
(12, 'pr', 1, 0, 15, 25, 0),
(12, 'corruption', 1, 0, 15, 25, 0),
(12, 'heroic_mr', 0, 20, 2, 10, 0),
(12, 'heroic_fr', 0, 20, 2, 10, 0),
(12, 'heroic_cr', 0, 20, 2, 10, 0),
(12, 'heroic_dr', 0, 20, 2, 10, 0),
(12, 'heroic_pr', 0, 20, 2, 10, 0),
(12, 'heroic_corruption', 0, 20, 2, 10, 0),
(12, 'spelldmg', 1, 0, 10, 30, 0),
(12, 'healamt', 1, 0, 1, 5, 0),
(12, 'attack', 1, 0, 1, 5, 0),
(12, 'hp_regen', 1, 0, 0, 3, 0),
(12, 'mana_regen', 1, 0, 0, 3, 0),
(12, 'spell_shield', 1, 0, 0, 3, 0),
(12, 'dot_shielding', 1, 0, 0, 3, 0),
(12, 'avoidance', 1, 0, 0, 3, 0),
(12, 'stun_resist', 1, 0, 0, 3, 0),
(12, 'spell_crit_chance_pct', 1, 0, 0, 6, 0),
(12, 'spell_crit_dmg_pct', 1, 0, 0, 10, 0),
(12, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(12, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(12, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(12, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(12, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(12, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(12, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Magician
(13, 'str', 1, 0, 3, 8, 0),
(13, 'sta', 1, 0, 9, 16, 0),
(13, 'dex', 1, 0, 3, 8, 0),
(13, 'agi', 1, 0, 3, 8, 0),
(13, 'int', 1, 0, 14, 26, 0),
(13, 'wis', 1, 0, 3, 8, 0),
(13, 'cha', 1, 0, 3, 8, 0),
(13, 'mr', 1, 0, 15, 25, 0),
(13, 'fr', 1, 0, 15, 25, 0),
(13, 'cr', 1, 0, 15, 25, 0),
(13, 'dr', 1, 0, 15, 25, 0),
(13, 'pr', 1, 0, 15, 25, 0),
(13, 'corruption', 1, 0, 15, 25, 0),
(13, 'heroic_mr', 0, 20, 2, 10, 0),
(13, 'heroic_fr', 0, 20, 2, 10, 0),
(13, 'heroic_cr', 0, 20, 2, 10, 0),
(13, 'heroic_dr', 0, 20, 2, 10, 0),
(13, 'heroic_pr', 0, 20, 2, 10, 0),
(13, 'heroic_corruption', 0, 20, 2, 10, 0),
(13, 'spelldmg', 1, 0, 10, 30, 0),
(13, 'healamt', 1, 0, 1, 5, 0),
(13, 'attack', 1, 0, 1, 5, 0),
(13, 'hp_regen', 1, 0, 0, 3, 0),
(13, 'mana_regen', 1, 0, 0, 3, 0),
(13, 'spell_shield', 1, 0, 0, 3, 0),
(13, 'dot_shielding', 1, 0, 0, 3, 0),
(13, 'avoidance', 1, 0, 0, 3, 0),
(13, 'stun_resist', 1, 0, 0, 3, 0),
(13, 'spell_crit_chance_pct', 1, 0, 0, 6, 0),
(13, 'spell_crit_dmg_pct', 1, 0, 0, 10, 0),
(13, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(13, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(13, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(13, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(13, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(13, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(13, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Enchanter
(14, 'str', 1, 0, 3, 8, 0),
(14, 'sta', 1, 0, 9, 16, 0),
(14, 'dex', 1, 0, 3, 8, 0),
(14, 'agi', 1, 0, 3, 8, 0),
(14, 'int', 1, 0, 14, 26, 0),
(14, 'wis', 1, 0, 3, 8, 0),
(14, 'cha', 1, 0, 14, 26, 0),
(14, 'mr', 1, 0, 15, 25, 0),
(14, 'fr', 1, 0, 15, 25, 0),
(14, 'cr', 1, 0, 15, 25, 0),
(14, 'dr', 1, 0, 15, 25, 0),
(14, 'pr', 1, 0, 15, 25, 0),
(14, 'corruption', 1, 0, 15, 25, 0),
(14, 'heroic_mr', 0, 20, 2, 10, 0),
(14, 'heroic_fr', 0, 20, 2, 10, 0),
(14, 'heroic_cr', 0, 20, 2, 10, 0),
(14, 'heroic_dr', 0, 20, 2, 10, 0),
(14, 'heroic_pr', 0, 20, 2, 10, 0),
(14, 'heroic_corruption', 0, 20, 2, 10, 0),
(14, 'spelldmg', 1, 0, 10, 30, 0),
(14, 'healamt', 1, 0, 1, 5, 0),
(14, 'attack', 1, 0, 1, 5, 0),
(14, 'hp_regen', 1, 0, 0, 3, 0),
(14, 'mana_regen', 1, 0, 0, 3, 0),
(14, 'spell_shield', 1, 0, 0, 3, 0),
(14, 'dot_shielding', 1, 0, 0, 3, 0),
(14, 'avoidance', 1, 0, 0, 3, 0),
(14, 'stun_resist', 1, 0, 0, 3, 0),
(14, 'spell_crit_chance_pct', 1, 0, 0, 4, 0),
(14, 'spell_crit_dmg_pct', 1, 0, 0, 6, 0),
(14, 'dot_crit_chance_pct', 1, 0, 0, 4, 0),
(14, 'dot_crit_dmg_pct', 1, 0, 0, 6, 0),
(14, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(14, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(14, 'melee_crit_chance_pct', 1, 0, 0, 2, 0),
(14, 'melee_crit_dmg_pct', 1, 0, 0, 3, 0),
(14, 'archery_crit_chance_pct', 1, 0, 0, 2, 0),
-- Beastlord
(15, 'str', 1, 0, 9, 16, 0),
(15, 'sta', 1, 0, 9, 16, 0),
(15, 'dex', 1, 0, 13, 23, 0),
(15, 'agi', 1, 0, 11, 22, 0),
(15, 'int', 1, 0, 3, 8, 0),
(15, 'wis', 1, 0, 6, 13, 0),
(15, 'cha', 1, 0, 3, 8, 0),
(15, 'mr', 1, 0, 15, 25, 0),
(15, 'fr', 1, 0, 15, 25, 0),
(15, 'cr', 1, 0, 15, 25, 0),
(15, 'dr', 1, 0, 15, 25, 0),
(15, 'pr', 1, 0, 15, 25, 0),
(15, 'corruption', 1, 0, 15, 25, 0),
(15, 'heroic_mr', 0, 20, 2, 10, 0),
(15, 'heroic_fr', 0, 20, 2, 10, 0),
(15, 'heroic_cr', 0, 20, 2, 10, 0),
(15, 'heroic_dr', 0, 20, 2, 10, 0),
(15, 'heroic_pr', 0, 20, 2, 10, 0),
(15, 'heroic_corruption', 0, 20, 2, 10, 0),
(15, 'spelldmg', 1, 0, 2, 8, 0),
(15, 'healamt', 1, 0, 3, 11, 0),
(15, 'attack', 1, 0, 9, 24, 0),
(15, 'hp_regen', 1, 0, 0, 3, 0),
(15, 'mana_regen', 1, 0, 0, 1, 0),
(15, 'end_regen', 1, 0, 0, 2, 0),
(15, 'combat_effects', 1, 0, 0, 2, 0),
(15, 'spell_shield', 1, 0, 0, 3, 0),
(15, 'shielding', 1, 0, 0, 2, 0),
(15, 'dot_shielding', 1, 0, 0, 3, 0),
(15, 'ds_mitigation', 1, 0, 0, 2, 0),
(15, 'avoidance', 1, 0, 0, 3, 0),
(15, 'accuracy', 1, 0, 0, 2, 0),
(15, 'stun_resist', 1, 0, 0, 3, 0),
(15, 'strike_through', 1, 0, 0, 2, 0),
(15, 'spell_crit_chance_pct', 1, 0, 0, 3, 0),
(15, 'spell_crit_dmg_pct', 1, 0, 0, 4, 0),
(15, 'dot_crit_chance_pct', 1, 0, 0, 3, 0),
(15, 'dot_crit_dmg_pct', 1, 0, 0, 4, 0),
(15, 'heal_crit_chance_pct', 1, 0, 0, 3, 0),
(15, 'hot_crit_chance_pct', 1, 0, 0, 3, 0);
)"
			R"(
INSERT IGNORE INTO `dynamic_affix_pool` (`class_id`, `affix`, `guaranteed`, `weight`, `min_value`, `max_value`, `slot_mask`) VALUES
(15, 'melee_crit_chance_pct', 1, 0, 0, 5, 0),
(15, 'melee_crit_dmg_pct', 1, 0, 0, 8, 0),
(15, 'archery_crit_chance_pct', 1, 0, 0, 4, 0),
-- Berserker
(16, 'str', 1, 0, 14, 26, 0),
(16, 'sta', 1, 0, 14, 26, 0),
(16, 'dex', 1, 0, 16, 28, 0),
(16, 'agi', 1, 0, 9, 16, 0),
(16, 'int', 1, 0, 3, 8, 0),
(16, 'wis', 1, 0, 3, 8, 0),
(16, 'cha', 1, 0, 3, 8, 0),
(16, 'mr', 1, 0, 15, 25, 0),
(16, 'fr', 1, 0, 15, 25, 0),
(16, 'cr', 1, 0, 15, 25, 0),
(16, 'dr', 1, 0, 15, 25, 0),
(16, 'pr', 1, 0, 15, 25, 0),
(16, 'corruption', 1, 0, 15, 25, 0),
(16, 'heroic_mr', 0, 20, 2, 10, 0),
(16, 'heroic_fr', 0, 20, 2, 10, 0),
(16, 'heroic_cr', 0, 20, 2, 10, 0),
(16, 'heroic_dr', 0, 20, 2, 10, 0),
(16, 'heroic_pr', 0, 20, 2, 10, 0),
(16, 'heroic_corruption', 0, 20, 2, 10, 0),
(16, 'spelldmg', 1, 0, 1, 5, 0),
(16, 'healamt', 1, 0, 1, 5, 0),
(16, 'attack', 1, 0, 12, 30, 0),
(16, 'hp_regen', 1, 0, 0, 3, 0),
(16, 'end_regen', 1, 0, 0, 3, 0),
(16, 'combat_effects', 1, 0, 0, 3, 0),
(16, 'spell_shield', 1, 0, 0, 3, 0),
(16, 'shielding', 1, 0, 0, 3, 0),
(16, 'dot_shielding', 1, 0, 0, 3, 0),
(16, 'ds_mitigation', 1, 0, 0, 3, 0),
(16, 'avoidance', 1, 0, 0, 3, 0),
(16, 'accuracy', 1, 0, 0, 3, 0),
(16, 'stun_resist', 1, 0, 0, 3, 0),
(16, 'strike_through', 1, 0, 0, 3, 0),
(16, 'spell_crit_chance_pct', 1, 0, 0, 2, 0),
(16, 'spell_crit_dmg_pct', 1, 0, 0, 3, 0),
(16, 'dot_crit_chance_pct', 1, 0, 0, 2, 0),
(16, 'dot_crit_dmg_pct', 1, 0, 0, 3, 0),
(16, 'heal_crit_chance_pct', 1, 0, 0, 2, 0),
(16, 'hot_crit_chance_pct', 1, 0, 0, 2, 0),
(16, 'melee_crit_chance_pct', 1, 0, 0, 6, 0),
(16, 'melee_crit_dmg_pct', 1, 0, 0, 10, 0),
(16, 'archery_crit_chance_pct', 1, 0, 0, 2, 0);
)"
			R"(
INSERT IGNORE INTO `dynamic_affix_suffixes` (`affix`, `suffix`) VALUES
('mr', 'of Spell Warding'),
('fr', 'of Warmth'),
('cr', 'of Insulation'),
('dr', 'of Health'),
('pr', 'of Antivenom'),
('corruption', 'of Purity'),
('heroic_mr', 'of the Magus'),
('heroic_fr', 'of the Salamander'),
('heroic_cr', 'of the Yeti'),
('heroic_dr', 'of the Healer'),
('heroic_pr', 'of the Serpent'),
('heroic_corruption', 'of the Saint');
)",
		.content_schema_update = true,
	},
	ManifestEntry{
		.version = 4,
		.description = "2026_10_22_armor_set_bonus_affixes",
		.check = "SELECT 1 FROM information_schema.columns WHERE table_schema = DATABASE() AND table_name = 'armor_set_bonuses' AND column_name = 'affix'",
		.condition = "empty",
		.match = "",
		.sql = R"(
CREATE TABLE IF NOT EXISTS `armor_set_items` (
  `item_id` INT(11) NOT NULL,
  `set_id` INT(11) NOT NULL,
  `class_mask` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`item_id`)
);

CREATE TABLE IF NOT EXISTS `armor_set_bonuses` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `set_id` INT(11) NOT NULL,
  `min_pieces` INT(11) NOT NULL,
  `class_mask` INT(11) NOT NULL,
  `stat_id` INT(11) NOT NULL,
  `stat_value` INT(11) NOT NULL,
  PRIMARY KEY (`id`)
);

ALTER TABLE `armor_set_bonuses` ADD COLUMN IF NOT EXISTS `affix` VARCHAR(32) NOT NULL DEFAULT '' AFTER `stat_id`;
)",
		.content_schema_update = true,
	},
	ManifestEntry{
		.version = 5,
		.description = "2026_10_23_dynamic_items_bis_rebalance",
		.check = "SELECT 1 FROM dynamic_affix_pool WHERE class_id = 3 AND affix = 'heroic_mr' AND guaranteed = 1",
		.condition = "empty",
		.match = "",
		.sql = R"(
DELETE FROM dynamic_affix_pool WHERE class_id = 16 AND affix IN ('skill_1hb_pct', 'skill_1hs_pct');
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 124, max_value = 161 WHERE class_id = 1 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 23 WHERE class_id = 1 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 50 WHERE class_id = 1 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 44 WHERE class_id = 1 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 1 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 1 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 1 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 33, max_value = 40 WHERE class_id = 1 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 1 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 1 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 1 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 1 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 1 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 1 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 1 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 1 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 424, max_value = 518 WHERE class_id = 1 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 1 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 1 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 1 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 1 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 27 WHERE class_id = 1 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 37 WHERE class_id = 1 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 49, max_value = 61 WHERE class_id = 1 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 1 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 124, max_value = 161 WHERE class_id = 2 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 2 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 2 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 2 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 2 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 2 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 2 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 2 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 2 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 33, max_value = 53 WHERE class_id = 2 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 2 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 2 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 2 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 2 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 2 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 2 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 2 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 2 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 2 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 2 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 37 WHERE class_id = 2 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 2 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 51 WHERE class_id = 2 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 34, max_value = 46 WHERE class_id = 2 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 111, max_value = 145 WHERE class_id = 3 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 15 WHERE class_id = 3 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 35, max_value = 46 WHERE class_id = 3 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 38 WHERE class_id = 3 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 3 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 3 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 3 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 38 WHERE class_id = 3 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 3 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 3 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 34 WHERE class_id = 3 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 3 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 3 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 3 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 3 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 3 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 406, max_value = 497 WHERE class_id = 3 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 3 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 3 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 3 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 3 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 30 WHERE class_id = 3 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 35 WHERE class_id = 3 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 48, max_value = 59 WHERE class_id = 3 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 33 WHERE class_id = 3 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 94, max_value = 125 WHERE class_id = 4 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 15 WHERE class_id = 4 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 35, max_value = 46 WHERE class_id = 4 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 38 WHERE class_id = 4 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 4 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 4 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 4 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 37, max_value = 47 WHERE class_id = 4 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 4 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 4 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 34 WHERE class_id = 4 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 4 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 4 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 4 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 4 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 4 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 406, max_value = 497 WHERE class_id = 4 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 4 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 4 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 4 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 4 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 30 WHERE class_id = 4 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 4 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 43, max_value = 49 WHERE class_id = 4 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 33 WHERE class_id = 4 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 99, max_value = 131 WHERE class_id = 5 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 15 WHERE class_id = 5 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 35, max_value = 46 WHERE class_id = 5 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 38 WHERE class_id = 5 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 5 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 5 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 5 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 38 WHERE class_id = 5 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 5 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 5 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 5 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 5 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 5 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 5 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 5 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 5 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 406, max_value = 497 WHERE class_id = 5 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 5 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 31 WHERE class_id = 5 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 5 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 5 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 33 WHERE class_id = 5 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 35 WHERE class_id = 5 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 46, max_value = 57 WHERE class_id = 5 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 5 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 96, max_value = 130 WHERE class_id = 6 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 6 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 6 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 6 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 6 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 6 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 6 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 6 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 6 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 33, max_value = 53 WHERE class_id = 6 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 6 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 6 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 6 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 6 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 6 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 6 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 6 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 6 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 6 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 6 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 37 WHERE class_id = 6 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 6 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 43 WHERE class_id = 6 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 34, max_value = 46 WHERE class_id = 6 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 96, max_value = 130 WHERE class_id = 7 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 23 WHERE class_id = 7 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 50 WHERE class_id = 7 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 44 WHERE class_id = 7 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 7 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 7 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 7 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 40, max_value = 52 WHERE class_id = 7 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 7 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 7 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 7 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 7 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 7 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 7 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 7 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 7 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 424, max_value = 518 WHERE class_id = 7 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 7 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 7 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 7 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 7 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 27 WHERE class_id = 7 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 7 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 49, max_value = 61 WHERE class_id = 7 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 7 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 88, max_value = 117 WHERE class_id = 8 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 15 WHERE class_id = 8 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 35, max_value = 46 WHERE class_id = 8 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 38 WHERE class_id = 8 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 8 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 29, max_value = 41 WHERE class_id = 8 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 8 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 37, max_value = 47 WHERE class_id = 8 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 8 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 8 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 8 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 8 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 8 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 8 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 8 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 8 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 406, max_value = 497 WHERE class_id = 8 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 8 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 31 WHERE class_id = 8 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 8 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 8 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 33 WHERE class_id = 8 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 8 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 43, max_value = 49 WHERE class_id = 8 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 8 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 108, max_value = 142 WHERE class_id = 9 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 23 WHERE class_id = 9 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 50 WHERE class_id = 9 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 44 WHERE class_id = 9 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 9 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 9 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 9 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 40, max_value = 52 WHERE class_id = 9 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 9 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 9 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 9 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 9 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 9 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 9 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 9 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 9 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 424, max_value = 518 WHERE class_id = 9 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 9 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 9 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 9 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 9 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 27 WHERE class_id = 9 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 9 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 51 WHERE class_id = 9 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 9 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 108, max_value = 142 WHERE class_id = 10 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 10 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 10 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 10 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 10 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 10 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 10 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 10 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 10 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 33, max_value = 53 WHERE class_id = 10 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 10 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 10 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 10 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 10 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 10 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 10 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 10 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 10 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 10 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 10 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 37 WHERE class_id = 10 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 10 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 51 WHERE class_id = 10 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 34, max_value = 46 WHERE class_id = 10 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 69, max_value = 99 WHERE class_id = 11 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 11 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 11 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 11 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 11 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 11 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 11 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 11 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 11 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 11 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 11 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 11 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 11 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 11 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 11 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 11 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 11 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 44 WHERE class_id = 11 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 11 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 11 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 52 WHERE class_id = 11 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 11 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 43 WHERE class_id = 11 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 11 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 69, max_value = 99 WHERE class_id = 12 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 12 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 12 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 12 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 12 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 12 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 12 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 12 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 12 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 12 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 12 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 12 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 12 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 12 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 12 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 12 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 12 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 44 WHERE class_id = 12 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 12 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 12 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 52 WHERE class_id = 12 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 12 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 43 WHERE class_id = 12 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 12 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 69, max_value = 99 WHERE class_id = 13 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 13 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 13 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 13 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 13 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 13 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 13 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 13 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 13 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 13 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 13 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 13 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 13 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 13 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 13 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 13 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 13 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 44 WHERE class_id = 13 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 13 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 13 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 52 WHERE class_id = 13 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 13 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 43 WHERE class_id = 13 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 13 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 69, max_value = 99 WHERE class_id = 14 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 14 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 15, max_value = 19 WHERE class_id = 14 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 14 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 29, max_value = 41 WHERE class_id = 14 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 14 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 27, max_value = 32 WHERE class_id = 14 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 14 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 14 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 14 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 14 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 14 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 14 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 14 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 14 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 353, max_value = 436 WHERE class_id = 14 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 14 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 44 WHERE class_id = 14 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 14 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 14 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 32, max_value = 52 WHERE class_id = 14 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 14 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 38, max_value = 43 WHERE class_id = 14 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 14 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 97, max_value = 128 WHERE class_id = 15 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 15 WHERE class_id = 15 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 35, max_value = 46 WHERE class_id = 15 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 38 WHERE class_id = 15 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 15 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 15 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 15 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 37, max_value = 47 WHERE class_id = 15 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 15 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 15 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 34 WHERE class_id = 15 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 15 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 15 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 15 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 15 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 15 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 406, max_value = 497 WHERE class_id = 15 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 15 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 15 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 15 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 15 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 30 WHERE class_id = 15 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 20, max_value = 27 WHERE class_id = 15 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 51 WHERE class_id = 15 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 33 WHERE class_id = 15 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 108, max_value = 142 WHERE class_id = 16 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 23 WHERE class_id = 16 AND affix = 'accuracy';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 33, max_value = 40 WHERE class_id = 16 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 26, max_value = 44 WHERE class_id = 16 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 13 WHERE class_id = 16 AND affix = 'avoidance';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 18, max_value = 23 WHERE class_id = 16 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 16 AND affix = 'cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 40, max_value = 52 WHERE class_id = 16 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 16 AND affix = 'dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 16 AND affix = 'fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28 WHERE class_id = 16 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 16 AND affix = 'heroic_cr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 16 AND affix = 'heroic_dr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 16 AND affix = 'heroic_fr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 16 AND affix = 'heroic_mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 6, max_value = 12 WHERE class_id = 16 AND affix = 'heroic_pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 424, max_value = 518 WHERE class_id = 16 AND affix = 'hp';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 11 WHERE class_id = 16 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 21, max_value = 26 WHERE class_id = 16 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 16 AND affix = 'mr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 28, max_value = 42 WHERE class_id = 16 AND affix = 'pr';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 27 WHERE class_id = 16 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 37 WHERE class_id = 16 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 49, max_value = 61 WHERE class_id = 16 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 23, max_value = 28 WHERE class_id = 16 AND affix = 'wis';
UPDATE dynamic_affix_pool SET min_value = 265, max_value = 320
WHERE affix = 'mana' AND class_id IN (3, 4, 5, 8, 15) AND min_value = 180 AND max_value = 220;
)",
		.content_schema_update = true,
	},
	ManifestEntry{
		.version = 6,
		.description = "2026_10_23_armor_set_bonus_spells",
		.check = "SELECT 1 FROM information_schema.columns WHERE table_schema = DATABASE() AND table_name = 'armor_set_bonuses' AND column_name = 'spell_type'",
		.condition = "empty",
		.match = "",
		.sql = R"(
ALTER TABLE `armor_set_bonuses` ADD COLUMN IF NOT EXISTS `spell_id` INT(11) NOT NULL DEFAULT 0 AFTER `affix`;
ALTER TABLE `armor_set_bonuses` ADD COLUMN IF NOT EXISTS `spell_type` TINYINT(3) UNSIGNED NOT NULL DEFAULT 0 COMMENT '1 worn effect, 2 melee proc, 3 spell-cast (sympathetic) proc' AFTER `spell_id`;
)",
		.content_schema_update = true,
	},
	// Caster rebalance: INT/WIS, spell damage, heal amount, STA, CHA for the seven caster classes
	ManifestEntry{
		.version = 7,
		.description = "2026_10_24_dynamic_items_caster_rebalance",
		.check = "SELECT 1 FROM dynamic_affix_pool WHERE class_id = 12 AND affix = 'spelldmg' AND min_value = 47 AND max_value = 76",
		.condition = "empty",
		.match = "",
		.sql = R"(
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 80, max_value = 103, slot_mask = 0 WHERE class_id = 2 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 2 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 2 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 32, slot_mask = 0 WHERE class_id = 2 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 2 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 2 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 4, max_value = 6, slot_mask = 0 WHERE class_id = 2 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 2 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 37, max_value = 53, slot_mask = 0 WHERE class_id = 2 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 2 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 2 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 2 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 62, max_value = 83, slot_mask = 0 WHERE class_id = 6 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 6 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 6 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 32, slot_mask = 0 WHERE class_id = 6 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 6 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 6 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 2, max_value = 3, slot_mask = 0 WHERE class_id = 6 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 6 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 37, max_value = 53, slot_mask = 0 WHERE class_id = 6 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 6 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 10, max_value = 12, slot_mask = 0 WHERE class_id = 6 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 6 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 69, max_value = 91, slot_mask = 0 WHERE class_id = 10 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 10 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 10 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 32, slot_mask = 0 WHERE class_id = 10 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 10 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 10 AND affix = 'healamt';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 4, max_value = 6, slot_mask = 0 WHERE class_id = 10 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 10 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 37, max_value = 53, slot_mask = 0 WHERE class_id = 10 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 10 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 10 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 10 AND affix = 'wis';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 63, slot_mask = 0 WHERE class_id = 11 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 11 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 11 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 32, slot_mask = 0 WHERE class_id = 11 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 11 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 2, max_value = 3, slot_mask = 0 WHERE class_id = 11 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 11 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 11 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 11 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 11 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 10, max_value = 12, slot_mask = 0 WHERE class_id = 11 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 63, slot_mask = 0 WHERE class_id = 12 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 12 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 12 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 32, slot_mask = 0 WHERE class_id = 12 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 12 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 2, max_value = 3, slot_mask = 0 WHERE class_id = 12 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 12 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 12 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 12 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 12 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 10, max_value = 12, slot_mask = 0 WHERE class_id = 12 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 63, slot_mask = 0 WHERE class_id = 13 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 13 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 13 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 25, max_value = 32, slot_mask = 0 WHERE class_id = 13 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 13 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 2, max_value = 3, slot_mask = 0 WHERE class_id = 13 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 13 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 13 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 13 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 13 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 10, max_value = 12, slot_mask = 0 WHERE class_id = 13 AND affix = 'str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 63, slot_mask = 0 WHERE class_id = 14 AND affix = 'ac';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 14 AND affix = 'agi';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 11, max_value = 14, slot_mask = 0 WHERE class_id = 14 AND affix = 'attack';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 40, max_value = 56, slot_mask = 0 WHERE class_id = 14 AND affix = 'cha';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 24, max_value = 28, slot_mask = 0 WHERE class_id = 14 AND affix = 'dex';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 2, max_value = 3, slot_mask = 0 WHERE class_id = 14 AND affix = 'heroic_str';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 0, max_value = 3, slot_mask = 0 WHERE class_id = 14 AND affix = 'hp_regen';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 52, max_value = 71, slot_mask = 0 WHERE class_id = 14 AND affix = 'int';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 47, max_value = 76, slot_mask = 0 WHERE class_id = 14 AND affix = 'spelldmg';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 44, max_value = 59, slot_mask = 0 WHERE class_id = 14 AND affix = 'sta';
UPDATE dynamic_affix_pool SET guaranteed = 1, weight = 0, min_value = 10, max_value = 12, slot_mask = 0 WHERE class_id = 14 AND affix = 'str';
)",
		.content_schema_update = true,
	},

	// Nautilus Vault storage (zone/nms_vault.cpp). Item state is kept per slot, so a round trip returns the
	// same item: attuned, ornaments, custom_data (dynamic item seeds) and guid.
	ManifestEntry{
		.version = 8,
		.description = "2026_10_29_character_nms_vault",
		.check = "SHOW TABLES LIKE 'character_nms_vault'",
		.condition = "empty",
		.match = "",
		.sql = R"(
CREATE TABLE IF NOT EXISTS character_nms_vault (
    character_id INT UNSIGNED NOT NULL,
    slot INT NOT NULL,
    bag_slot INT NOT NULL DEFAULT 0,
    item_id INT UNSIGNED NOT NULL,
    charges SMALLINT NOT NULL DEFAULT 1,
    aug1 INT UNSIGNED NOT NULL DEFAULT 0,
    aug2 INT UNSIGNED NOT NULL DEFAULT 0,
    aug3 INT UNSIGNED NOT NULL DEFAULT 0,
    aug4 INT UNSIGNED NOT NULL DEFAULT 0,
    aug5 INT UNSIGNED NOT NULL DEFAULT 0,
    aug6 INT UNSIGNED NOT NULL DEFAULT 0,
    instnodrop TINYINT UNSIGNED NOT NULL DEFAULT 0,
    custom_data TEXT DEFAULT NULL,
    ornament_icon INT UNSIGNED NOT NULL DEFAULT 0,
    ornament_idfile INT UNSIGNED NOT NULL DEFAULT 0,
    ornament_hero_model INT UNSIGNED NOT NULL DEFAULT 0,
    guid BIGINT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (character_id, slot, bag_slot),
    KEY idx_character_nms_vault_slot (character_id, slot)
);
)",
		.content_schema_update = false,
	},

	// /nmsloot personal loot offers (zone/nms_loot_offers.cpp), kept so they survive zoning
	ManifestEntry{
		.version = 9,
		.description = "2026_10_29_character_nms_loot_offers",
		.check = "SHOW TABLES LIKE 'character_nms_loot_offers'",
		.condition = "empty",
		.match = "",
		.sql = R"(
CREATE TABLE IF NOT EXISTS character_nms_loot_offers (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    character_id INT UNSIGNED NOT NULL,
    zone_id INT UNSIGNED NOT NULL DEFAULT 0,
    instance_id INT UNSIGNED NOT NULL DEFAULT 0,
    corpse_id INT UNSIGNED NOT NULL DEFAULT 0,
    corpse_serial BIGINT UNSIGNED NOT NULL DEFAULT 0,
    item_id INT UNSIGNED NOT NULL,
    icon INT UNSIGNED NOT NULL DEFAULT 0,
    charges SMALLINT NOT NULL DEFAULT 1,
    bonus TINYINT UNSIGNED NOT NULL DEFAULT 0,
    name VARCHAR(64) NOT NULL DEFAULT '',
    aug1 INT UNSIGNED NOT NULL DEFAULT 0,
    aug2 INT UNSIGNED NOT NULL DEFAULT 0,
    aug3 INT UNSIGNED NOT NULL DEFAULT 0,
    aug4 INT UNSIGNED NOT NULL DEFAULT 0,
    aug5 INT UNSIGNED NOT NULL DEFAULT 0,
    aug6 INT UNSIGNED NOT NULL DEFAULT 0,
    passed TINYINT UNSIGNED NOT NULL DEFAULT 0,
    passed_from VARCHAR(64) NOT NULL DEFAULT '',
    expires_at TIMESTAMP NOT NULL,
    PRIMARY KEY (id),
    KEY idx_character_nms_loot_offers_char (character_id, zone_id, expires_at)
);
)",
		.content_schema_update = false,
	},
};
