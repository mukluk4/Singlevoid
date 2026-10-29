# Dynamic items and Greater Rifts: generators and scripts

The SQL in `utils/sql/dynamic_items/` was generated from these scripts against stock PEQ (database 9328).
Rerun them when you change a setting, add a rift zone, or want the SQL built from your own database.

## Generators

Every generator takes a **source** as its first argument:

- an `eqemu_config.json`: reads that server's database with the mysql client (set `MYSQL_EXE` if it isn't on the PATH);
- a mysqldump file, such as PEQ's `create_tables_content.sql`: loads the tables it needs into memory (SQLite). Nothing is written to any database.

The generators only **read**. They write SQL and Lua files, which you apply yourself.

| Script | Writes | From |
|---|---|---|
| `gen_class_sets.py <source> sql class_sets.json <out.sql>` | `02_class_sets.sql`: tier 1 class armor (16 classes x 7 pieces), set bonuses at 2/4/8 pieces, the Chronal Quartermaster's list | `class_sets.json` |
| `gen_tier_gear.py <source> class_sets.json tier_gear.json <out.sql>` | `03_tier_gear.sql`: armor tiers 2-5, jewelry and augments for tiers 1-5, their set bonuses and server-side spells, 14 tier vendors in The Void | `tier_gear.json` |
| `gen_rift_zones.py <source> <out.sql> rift/lua_modules/rift_zones.lua` | `04_rift_zones.sql` and the rift zone pool | the zone list in the script |
| `gen_rift_appearances.py <source> rift/lua_modules/rift_appearances.lua` | mob looks per rift zone, plus Guardian looks | the database's own spawns |
| `luacheck.py <lua51.dll> <files>` | nothing: compiles the Lua with the server's Lua to catch syntax errors | |
| `make_package.py [--base <commit>] [--no-bin]` | the release folder and zip in `build/package/`: the code patch (checked against a clean upstream copy), the content, and the built Windows programs | the working tree; nothing is committed |

`gen_class_sets.py <eqemu_config.json> export class_sets.json` writes the settings back out of a live
database that already has the sets (it reads NMS's id ranges).

**Order matters:** `03` copies the tier 1 armor from `02`, and both copy the shell item and the Quartermaster from `01_rift_base.sql`.

## Rift zones

`gen_rift_zones.py` gives each zone in its list a rift copy at a spare instance version:
- an Anachronism spawn at every normal spawn point, respawning every 180 seconds (+/-30), except near the arrival point;
- the invisible controller at the arrival point;
- the doors (unlocked), zone points and ground spawns;
- a zone row with no flag, level or status requirement.

It picks the first version from 1 up that holds nothing else, so a rift never mixes with existing
content. On stock PEQ, Hate, Grieg's End, Dreadspire and Nagafen's Lair use version 2. To add a zone:
1. Add it to `DEFAULT_ZONES`.
2. Give it an arrival heading in `ENTRY_HEADING` (0 north, 128 east, 256 south, 384 west).
3. Set its arrival point in `DEFAULT_ENTRY` if the zone's safe point is poor.
4. Rerun both rift generators and apply the SQL.

## Rift scripts (`rift/`)

| File | Copy to |
|---|---|
| `lua_modules/*.lua` | your server's `lua_modules/` folder (the one the server searches; check `directories.lua_modules` in `eqemu_config.json`) |
| `quests/global/Rift_*.lua` | `quests/global/` |
| `quests/thevoida/459000.lua` | `quests/thevoida/` (Zebuxoruk) |
| `GlobalLoad.txt` | each player's EverQuest `Resources/` folder (client file; loads the models the rift NPCs and Guardians use) |

Every rift setting (hub, time limit, tier cap, rewards, NPC ids, pylons, champion and Guardian
spells) is in `lua_modules/rift_config.lua`. The scripts need the stock quest modules
`string_ext.lua` and `items.lua`, which come with the PEQ quests.
