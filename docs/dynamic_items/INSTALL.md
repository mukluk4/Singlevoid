# Dynamic Items and Greater Rifts add-on: install

- **Dynamic items:** gear whose stats roll once, when bought from an alternate currency vendor. Only a seed is
  stored on the item, so retuning the affix tables retunes every copy already owned.
- **Armor sets:** set bonuses at piece thresholds, including worn effects and procs.
- **Greater Rifts:** timed, tiered runs through instanced copies of 28 zones, opened by Zebuxoruk in The Void. Clears pay
  Chronal Shards (and platinum), which buy class gear in five tiers from the vendors in The Void.
- **Optional:** the NMS AA set, and AAs from level 1.

Your database, maps and quests stay as they are. The package only adds rows in id ranges stock PEQ doesn't use.

## What it changes

| Part | Where | How it's applied |
|---|---|---|
| Server code | 5 new files in `zone/` and `common/`, changes to about 30 existing files | Compiled in |
| Tables | `dynamic_items`, `dynamic_affix_pool`, `dynamic_affix_suffixes`, `armor_set_items`, `armor_set_bonuses` | Migrations, when `world` starts |
| Version counter | `db_version.dynamic_items_version` | Separate from `custom_version`, so your own custom migrations never collide with these |
| Content | `utils/sql/dynamic_items/01-04_*.sql` | You run them |
| Rift scripts | `utils/scripts/dynamic_items/rift/` | You copy them into your quests and `lua_modules` |
| Client file | the player's `Resources/GlobalLoad.txt` | `rift/add_globalload.py` adds the rift models |
| Optional AA set | `utils/sql/dynamic_items/nms_aa_*.sql` | You run them, then turn on a rule |

Every rule defaults to stock behavior.

## Install

1. **Back up your database.** Make a full dump first.
2. **Apply the code.** Merge the `dynamic-items` branch, or apply the patch, onto upstream EQEmu
   (built against database version 9328). Expect to merge by hand where your own server code touches the same functions.
3. **Build** `zone` and `world`.
4. **Start `world` once.** It adds the `dynamic_items_version` column and runs the 7 migrations
   (tables, affix pool, suffixes, set bonus columns, tuned stat ranges). The log shows
   `DynItems | database [7] binary [7] up to date`. Zones wait until `world` has run them.
5. **Shut the server down** and load the content, in order:
   - `00_check_ids.sql`: only if your database isn't stock PEQ. Every count should be 0.
   - `01_rift_base.sql`: the shell item, the Chronal Shard currency, the rift NPCs and spawngroups, and Zebuxoruk. It also opens The Void.
   - `02_class_sets.sql`: tier 1 class armor (16 classes x 7 pieces), set bonuses, and the Chronal Quartermaster's list.
   - `03_tier_gear.sql`: armor tiers 2-5, jewelry and augments for tiers 1-5, their set bonuses, 24 server-side spells, and 14 tier vendors.
   - `04_rift_zones.sql`: the rift copies of the 28 zones.
6. **Run `shared_memory`**: the new items and spells load from it.
7. **Copy the rift scripts** (see `utils/scripts/dynamic_items/README.md`):
   - `rift/lua_modules/*.lua` into your `lua_modules` folder;
   - `rift/quests/global/Rift_*.lua` into `quests/global/`;
   - `rift/quests/thevoida/459000.lua` into `quests/thevoida/`.
8. **Players** run `rift/add_globalload.py` on their `Resources/GlobalLoad.txt`, so the Guardian, Pylon and Portal models render.
   Without it those NPCs show as a placeholder model; nothing else breaks.
9. **Start the server** and set any rules you want.

The SQL was generated from stock PEQ. If your database differs a lot, regenerate it from your own database
(see the scripts README): the generators read your spawns, zones and items, and only write files.

## Greater Rifts

- **Opening one:** hail Zebuxoruk in The Void and pick a tier. Group members standing in The Void come along.
- **Tiers:** a character can open any tier up to one past their deepest clear (cap 500).
- **The run:** a random zone from the pool, full of Anachronisms that scale with the tier.
  - Kills fill a progress bar; pylons spawn at 25/50/75%.
  - At 100% the Rift Guardian appears.
  - Kill it within 15 minutes.
- **Rewards:** each player gets Chronal Shards (5 + 2 per tier, +25% under 10 minutes) and 1,000 platinum per tier.
  A clear one past their deepest raises their progression.
- **Gear unlocks:** the tier vendors hide their items until the character's deepest clear (`rift_deepest` bucket) reaches the unlock.
  Armor tier N opens at (N-1) x 100, jewelry 25 later, augments 75 later.
- **Settings:** every number above, plus the NPC ids, is in `rift/lua_modules/rift_config.lua`. Run `#reloadquest` after editing it.
- **The Void:** stock PEQ makes it GM-only; `01_rift_base.sql` opens it to everyone.
  To gate it, set `zone.flag_needed` for `thevoida`, or add a check in your own `global_player`.

## Rules

| Rule | Default | What it does |
|---|---|---|
| `Merchant:DynamicItemSellRefundPct` | 20 | Percent of the alternate currency cost refunded when a dynamic item is sold back |
| `Custom:DynamicItemDisplayIds` | true | Each rolled copy is sent to the client under its own item ID, so the item window shows that copy's stats |
| `Custom:ArmorSetTierVisuals` | `13,15,17,12` | Spell effects shown when a set bonus tier is gained (tiers 1, 2, 3) or lost |
| `Custom:KillFactionHits` | true | False stops kills from changing faction (quest faction still works) |
| `Custom:AutoTrainSkills` | false | Raise skills to their level cap on level-up and login |
| `Custom:AutoScribeSpells` | false | Scribe every class spell up to the character's level |
| `Custom:AutoLearnDisciplines` | false | Learn every class discipline up to the character's level |
| `Custom:AAStartLevel` | 51 | Level at which characters earn and buy AAs. 1 starts AAs at the beginning; AA ranks that need level 51 or less then open at this level. Range 1-51 |
| `Custom:NmsAATables` | false | Load AAs from the `nms_aa_*` tables instead of `aa_*` |
| `Custom:NmsCustomAAs` | false | With `NmsAATables` on, also load the AAs NMS added |

Zones load AAs when they boot, so the three AA rules need a zone restart.

## Your own dynamic items

A dynamic item is an ordinary `items` row with no stats of its own, plus a `dynamic_items` row:

```sql
-- 1) Copy the shell (900101) to a free item id; set its Name, slots, icon and look from a real item
-- 2) Register it
INSERT INTO dynamic_items (item_id, class_id, tier, min_affixes, max_affixes, stat_multiplier, notes)
VALUES (<your item id>, 1, 1, 2, 4, 1.0, 'Warrior chest');
-- 3) Sell it: merchantlist row with alt_currency_cost, on an NPC with alt_currency_id set
```

- **`class_id`:** the one class that can buy it (1 Warrior to 16 Berserker).
- **`stat_multiplier`:** scales every roll.
- **Ranges:** each class's stats and ranges are in `dynamic_affix_pool`. `guaranteed = 1` rows always roll; the others are picked by `weight`.
- **Unlocks:** merchant rows can be gated with `bucket_name` / `bucket_value` / `bucket_comparison`, on alternate currency vendors too.
- **Armor sets:** list the pieces in `armor_set_items` and the bonuses in `armor_set_bonuses`.
- **Easier route:** edit `class_sets.json` / `tier_gear.json` and regenerate.

GM commands: `#dynitem` (give, info, reroll, setseed, reload). Players: `#dyninfo`.

## Optional: the NMS AA set

This is NMS's version of the AA tables, compared with stock PEQ 9328:

| Table | Added | Removed | Changed |
|---|---|---|---|
| aa_ability | 10 | 11 | 62, mostly which classes can buy them |
| aa_ranks | 30 | 10 | 6,642: level requirements (most max out at 80), post-cap expansion ranks hidden, recast and cost changes |
| aa_rank_effects | 53 | 41 | 273 |
| aa_rank_prereqs | 107 | 432 | 1 |

1. Load `nms_aa_tables.sql`. It drops and recreates only the `nms_aa_*` tables.
2. Set `Custom:NmsAATables` to true and restart the zones. If any `nms_aa_*` table is missing, zones log an error and load the stock tables.
3. Set it back to false to return to stock AAs. Nothing in `aa_*` changed.

**Custom AAs (`Custom:NmsCustomAAs`)**: Dimensional Armory, Consume Item, Consume Experience, Situational Awareness,
Way of Steel, Alternately Advanced Firework, Necropotence, Beeeees!, Shadow Curse, Shamanic Tongues.
These are NMS-only and **not plug-and-play**:
- they need the spells in `nms_aa_custom_spells.sql` (run `shared_memory` after loading it);
- players need NMS client files (spell and AA text);
- Dimensional Armory summons NMS item 900000, and Consume Item / Consume Experience are driven by NMS quest scripts;
- Situational Awareness is grant-only here.

**Characters**: AA ranks bought under one set stay saved when you switch. Ranks the other set doesn't have are ignored while it's active, and the points spent on them aren't refunded. Pick a set before players start buying.

## Uninstall

1. Set the rules back to their defaults.
2. Remove the rift scripts.
3. The rows can stay; stock code ignores the tables. Owned dynamic items keep their seed in `custom_data` and act as their stat-less base items without the add-on code.
