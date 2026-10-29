# Dynamic Items and Greater Rifts add-on: install

- **Dynamic items:** gear whose stats roll once, when bought from an alternate currency vendor. Only a seed is
  stored on the item, so retuning the affix tables retunes every copy already owned.
- **Armor sets:** set bonuses at piece thresholds, including worn effects and procs.
- **Greater Rifts:** timed, tiered runs through instanced copies of 27 zones, opened by Zebuxoruk in The Void. Clears pay
  Chronal Shards (and platinum), which buy class gear in five tiers from the vendors in The Void.
- **Sinks:** reforging one line of a rolled item (`#reforge`), Chronal Tributes that only work inside rifts, and
  Paragon AAs (long, rising-cost AA lines).
- **Optional:** the NMS AA set, AAs from level 1, and a few AA and combat changes from Triptych, each behind its own rule.
- **Optional, needs the NMS client add-on:** the Nautilus Vault (extra storage, a Proc Locker, remote bank and merchant)
  and `/nmsloot` (personal loot rolls for every group member). See [Nautilus Vault and /nmsloot](#nautilus-vault-and-nmsloot).

Your database, maps and quests stay as they are. The package only adds rows in id ranges stock PEQ doesn't use.

## What it changes

| Part | Where | How it's applied |
|---|---|---|
| Server code | 9 new files in `zone/` and `common/`, changes to about 35 existing files | Compiled in |
| Tables | `dynamic_items`, `dynamic_affix_pool`, `dynamic_affix_suffixes`, `armor_set_items`, `armor_set_bonuses` | Migrations, when `world` starts |
| Version counter | `db_version.dynamic_items_version` | Separate from `custom_version`, so your own custom migrations never collide with these |
| Content | `utils/sql/dynamic_items/01-05_*.sql` | You run them |
| Rift scripts | `utils/scripts/dynamic_items/rift/` | You copy them into your quests and `lua_modules` |
| Client files | the player's `Resources/GlobalLoad.txt`, `spells_us.txt` and `Resources/dbstr_us.txt` | `rift/add_globalload.py`, `rift/client/add_spells.py` and `rift/client/add_dbstr.py` |
| Optional AA set | `utils/sql/dynamic_items/nms_aa_*.sql` | You run them, then turn on a rule |
| Vault and `/nmsloot` | two tables (migrations), `07_vault_key.sql`, two opcodes in `utils/patches/patch_RoF2.conf` | Rules, plus the NMS client add-on on each player's PC |

Every rule defaults to stock behavior.

## Install

1. **Back up your database.** Make a full dump first.
2. **Apply the code.** Merge the `dynamic-items` branch, or apply the patch, onto upstream EQEmu
   (built against database version 9328). Expect to merge by hand where your own server code touches the same functions.
3. **Build** `zone` and `world`.
4. **Start `world` once.** It adds the `dynamic_items_version` column and runs the 9 migrations
   (tables, affix pool, suffixes, set bonus columns, tuned stat ranges, the vault and loot offer tables). The log shows
   `DynItems | database [9] binary [9] up to date`. Zones wait until `world` has run them.
5. **Shut the server down** and load the content, in order:
   - `00_check_ids.sql`: only if your database isn't stock PEQ. Every count should be 0.
   - `01_rift_base.sql`: the shell item, the Chronal Shard currency, the rift NPCs and spawngroups, and Zebuxoruk. It also opens The Void.
   - `02_class_sets.sql`: tier 1 class armor (16 classes x 7 pieces), set bonuses, and the Chronal Quartermaster's list.
   - `03_tier_gear.sql`: armor tiers 2-5, jewelry and augments for tiers 1-5, their set bonuses, 34 custom spells, and 14 tier vendors.
   - `04_rift_zones.sql`: the rift copies of the 27 zones.
   - `05_sinks.sql`: Chronal Tributes 90-93, the Paragon AAs and the Chronal Artificer (`#reforge`) in The Void.
   - `06_paragon_nms_aa.sql`: only if you use the NMS AA set (`Custom:NmsAATables`); run it after `nms_aa_tables.sql`.
   - `07_vault_key.sql`: only if you turn on the Nautilus Vault.
6. **Run `shared_memory`**: the new items and spells load from it.
7. **Copy the rift scripts** (see `utils/scripts/dynamic_items/README.md`):
   - `rift/lua_modules/*.lua` into your `lua_modules` folder;
   - `rift/quests/global/Rift_*.lua` into `quests/global/`;
   - `rift/quests/thevoida/459000.lua` (Zebuxoruk) and `1200050.lua` (the Chronal Artificer) into `quests/thevoida/`.
8. **Players** patch three client files (each script keeps a `.bak` of the original):
   - `rift/add_globalload.py` on `Resources/GlobalLoad.txt`, so the Guardian, Pylon and Portal models render.
     Without it those NPCs show as a placeholder model; nothing else breaks.
   - `rift/client/add_spells.py` on `spells_us.txt`, for the Ranger jewelry proc Hail of Arrows I-V (spells 43021-43025).
     Without it the proc still fires and hits, but the client doesn't know the spell.
   - `rift/client/add_dbstr.py` on `Resources/dbstr_us.txt` (and on `dbstr_us.txt` in the client folder, if there is
     one), for the Paragon AA names and descriptions and the Chronal Shard currency name.
     Without it the AA window shows those AAs without text, and the currency shows as "Unknown DB String 2-18".
9. **Start the server** and set any rules you want.
    Two more things players on a stock RoF2 client will notice:
   - **The Void's vendors** (Clockwork Gnome, Clockwork Golem and Junk Beast models) can show as the placeholder
     model: a stock client's `thevoida_chr.txt` loads only The Void's own models. Add the models to that file
     (the count on line 1 goes up by one per line), or give players a client with them already in it.
   - **Unknown currencies:** the currency window lists every row of `alternate_currency`, and a stock client has no
     name for many of PEQ's (they show as "Unknown DB String N-18"). If your server uses no currency but Chronal
     Shards, back up the table and run `DELETE FROM alternate_currency WHERE id <> 2;`, then restart the zones.
10. **Vault and `/nmsloot` only:** the patch adds their two opcodes to `utils/patches/patch_RoF2.conf`. If your server
    runs from a copy of that file (next to the binaries), add the two `OP_NmsLoot*` lines to that copy too.

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
- **Resists:** rift NPC spells are harder to resist from tier 1 (400 + 2.8 per tier, at most 1,200), so stacking
  resists never makes a character immune.
- **Zone crashes:** progress, pylons, the Guardian and the exit portal are saved in a data bucket as the run goes, and
  come back when the rift zone restarts.
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
| `Custom:DynamicItemPctCap` | 100 | Cap on each dynamic item percent total (skill %, ability damage %, spell type damage %, heal %, instrument %, crit damage %). 0 = no cap |
| `Custom:DynamicItemCritChancePctCap` | 50 | Cap on each dynamic item crit chance total. 0 = no cap |
| `Pets:ScaleExcessPetPower` | false | Pet power above the strongest `pets` row for a pet type scales that pet's HP, AC, damage and level, as a -1 row does. The Void Conduit jewelry bonus is built for this |
| `Custom:ReforgeNPCID` | 1200050 | NPC `#reforge` is used at (target it, within 30 units). 0 = anywhere |
| `Custom:ReforgePlatBase` / `ReforgeCurrencyBase` | 25000 / 50 | Cost of the first reforge of a tier 1 item; tier N costs this x (2N - 1) |
| `Custom:ReforgeCurrencyID` | 2 | Alternate currency `#reforge` charges (Chronal Shards) |
| `Custom:ReforgeCostGrowthPct` / `ReforgeCostMaxPct` | 25 / 500 | Each reforge of the same item costs this much more, up to this percent of the first |
| `Custom:ChronalTributeFirstID` / `LastID` | 90 / 99 | Tribute ids that are Chronal Tributes |
| `Custom:ChronalTributeBandSize` | 100 | Rift tiers per Chronal Tribute tier |
| `Custom:ChronalTributeFavorPerTier` | 100 | Favor per Chronal Tribute per run: this x the rift tier |
| `Custom:AAFocusEffectsStack` | false | AA focus effects sum across AA lines instead of the highest one counting |
| `Custom:AACritDmgNoStackComponentsStack` | false | AA spell crit damage (SPA 294) sums across AA lines |
| `Custom:DoTCritsShareNoStackPool` | false | DoT crits also get that summed AA crit damage |
| `Custom:MaxAACritDmgFocus` | 500 | With those on: cap on the summed AA crit damage and damage focus, in percent |
| `Custom:RogueRangerInnateCrit` | false | Rogues and Rangers 12+ can crit with every skill (stock: Rangers only with Archery, Rogues only with Throwing) |
| `Custom:ThrowingUsesArcheryBonuses` | false | Archery accuracy also applies to Throwing, and Throwing can Headshot |
| `Custom:PetCritsAtLeastDouble` | false | A pet crit always deals at least double damage |
| `Custom:BestialAlignmentRacialLook` | false | Bestial Alignment turns each recipient into its own race's warder, not the generic wolf |
| `Custom:NautilusVault` | false | The Nautilus Vault, Proc Locker and vault key. Needs the NMS client add-on |
| `Custom:ProcLockerStacks` | true | Proc Locker items proc on their own rolls beside the held weapon. False: the locker item replaces the held weapon's proc |
| `Custom:ProcLockerModSlots` | true | Vault slots 84-87 lend their item's skill mods, instrument mods and focus effects |
| `Custom:NmsLootOffers` | false | `/nmsloot` personal loot rolls. Needs the NMS client add-on |
| `Custom:NmsLootOfferExpireSeconds` | 300 | Seconds an unclaimed `/nmsloot` offer lasts |
| `Custom:NmsLootEmptyGroupCorpse` | true | With offers going to 2+ players, the corpse's loot table items are removed (quest items stay) |

Zones load AAs when they boot, so the three AA rules need a zone restart.

## Sinks

- **`#reforge`:** at the Chronal Artificer in The Void (hail it for a link). Pay platinum and Chronal Shards to reroll one
  random line of a dynamic item, then take one of two new lines or keep the old one. Once a line is reforged it's the
  only one on that item that can be, and each reforge of the same item costs more. The choice is stored with the item
  (`custom_data`), so it survives zoning.
- **Chronal Tributes** (Might, Arcana, Bulwark, Fortune): bought at a tribute master like any tribute, but they only
  work inside a Greater Rift. Each tribute tier covers 100 rift tiers and the tiers stack. Instead of the usual cost
  every 10 minutes, each takes 100 favor x the rift tier once per run. Fortune adds 5% shards per tribute tier to a clear.
- **Paragon AAs:** 12 lines (HP, mana, endurance, AC, attack, melee damage, heroic stats) with rising costs, open from
  level 51. The heroic stat lines use effect ids 9001-9006, which only the server knows; they aren't sent to the client.

## Nautilus Vault and /nmsloot

Both are optional and off by default. Their windows are drawn by the **NMS client add-on**, a `dinput8.dll` that sits
next to `eqgame.exe` on each player's PC. The server side is in this package; the client side is not.

- **Nautilus Vault** (`Custom:NautilusVault`): each character gets a vault key item (9011009) on zone-in. Right-clicking
  it opens the vault window: 9 pages of storage (items keep their augments, ornaments, attunement and dynamic item rolls),
  a Proc Locker (slots 81-83: an item there procs as if held in primary, secondary or ranged), mod slots (84-87: skill
  mods, instrument mods and focus effects), clicky autoload (61-80), and a bank and merchant you can open from anywhere.
  Without the add-on the key does nothing; the `#vault_*` commands still work, but they take raw slot numbers.
- **`/nmsloot`** (`Custom:NmsLootOffers`): when an NPC dies, its loot is rolled again for the killer and each group or
  raid member in the zone, and sent to their `/nmsloot` window. Each player chooses Keep, Sell, Tribute, Bank, Vault,
  Destroy or Pass from anywhere in the zone; undecided offers come back after zoning until they expire. With 2 or more
  players rolling, the corpse's own table loot is removed (`Custom:NmsLootEmptyGroupCorpse`). Without the add-on a
  player never sees their offers, so **only turn this on if every player has the add-on.**

### Getting the add-on to players

The add-on comes from the NMS release ([tunaria/NMS-Release](https://github.com/tunaria/NMS-Release), MIT licensed,
source included): `Release-NMS-Client/ClientFiles/` holds the built `dinput8.dll` and the UI files it uses. It works
on the same RoF2 client as stock EQEmu. Its other features (the multi-pet window, waypoints, multiclass `/who`) wait
for packets a stock server never sends, so they stay idle; its client-side fixes (8-digit item links, discipline
timers) work anywhere.

What a player needs, on top of their RoF2 client:

| File | From | Why |
|---|---|---|
| `dinput8.dll` and `uifiles\` | NMS `Release-NMS-Client/ClientFiles/` | the vault and `/nmsloot` windows |
| `spells_us.txt`, `dbstr_us.txt` | your server's own files, patched with `rift/client/add_spells.py` and `add_dbstr.py` | Hail of Arrows, Paragon AA text, Chronal Shard name |
| `Resources/GlobalLoad.txt` | patched with `rift/add_globalload.py` | rift NPC models |

The simplest way is to patch one client copy yourself and hand players a zip of just those files, to copy over their
own client. A client patcher that downloads changed files works too.

- **Anti-virus:** some anti-virus products flag any unsigned DLL that hooks a game. Players may need to allow it, or
  build it themselves from the source.
- **Oddities on a stock server:** at zone-in the add-on prints "WARNING! -- You are on a NON Multiclass Server
  with the Multiclass Client", and character select shows extra class icons (it reads the deity field as a class
  list). Neither stops the vault or `/nmsloot` from working.
- **One client, two servers:** the DLL can stay when a player also plays elsewhere, but the spell, string and skill
  files must match the server being played. Keep a separate client folder per server.

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
2. Load `nms_aa_spells.sql`: NMS's versions of the 463 spells these AAs cast or proc (Holy Shield's rune, Rabid Bear,
   the Elemental Forms, Bazaar and Back...). Without it those AAs cast the stock spells and behave differently. It
   overwrites those rows in `spells_new`, and 24 of the spells are also used by stock NPCs, items or scrolls, so back
   up `spells_new` first. Then run `shared_memory`, and give players a `spells_us.txt` exported after loading it.
3. Set `Custom:NmsAATables` to true and restart the zones. If any `nms_aa_*` table is missing, zones log an error and load the stock tables.
4. Set it back to false to return to stock AAs. Nothing in `aa_*` changed; restore your `spells_new` backup to undo step 2.

**AA names and descriptions**: NMS renamed some AAs (Improved Shield Specialist became Holy Shield, Decapitation
became Cleaving Frenzy...) and rewrote many descriptions. Those strings live in the client's `dbstr_us.txt`, which
this package doesn't include, so players see the stock names and text unless you give them updated strings.

**Custom AAs (`Custom:NmsCustomAAs`)**: Dimensional Armory, Consume Item, Consume Experience, Situational Awareness,
Way of Steel, Alternately Advanced Firework, Necropotence, Beeeees!, Shadow Curse, Shamanic Tongues.
These are NMS-only and **not plug-and-play**:
- they need the spells in `nms_aa_custom_spells.sql` (run `shared_memory` after loading it);
- players need NMS client files (spell and AA text);
- Dimensional Armory summons NMS item 900000, and Consume Item / Consume Experience are driven by NMS quest scripts;
- Situational Awareness is grant-only here.

**Characters**: AA ranks bought under one set stay saved when you switch. Ranks the other set doesn't have are ignored while it's active, and the points spent on them aren't refunded. Pick a set before players start buying.

## Uninstall

1. Set the rules back to their defaults. Empty the Nautilus Vaults first if you used them: items in `character_nms_vault`
   are only reachable through the vault.
2. Remove the rift scripts.
3. The rows can stay; stock code ignores the tables. Owned dynamic items keep their seed in `custom_data` and act as their stat-less base items without the add-on code.
