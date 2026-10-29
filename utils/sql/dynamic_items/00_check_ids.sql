-- Dynamic items + Greater Rifts: run this BEFORE 01-04 on a database that isn't stock PEQ.
-- It lists rows of yours that the package would overwrite or delete. Every count should be 0.
-- (Once the package is installed its own spells count here; the other checks skip the package's rows.)
-- Ranges: items 800002, 900101-900916, 940000-964999; spells 50001-50024; npc_types / merchants 1200000-1200050;
-- spawngroups 5003834, 5003835, 5003837; alternate currency 2; armor sets 101-516 and 1101-1516.
-- Optional files: 08 (spells 17779, 36856, 43002, 43005), 09 (npc_types 1200060, spawngroup 5003853).

SELECT 'items' AS what, COUNT(*) AS yours FROM items
 WHERE (id = 800002 AND Name <> 'Chronal Shard')
    OR (id BETWEEN 900101 AND 900916 AND id NOT IN (SELECT item_id FROM dynamic_items) AND id <> 900101)
    OR (id BETWEEN 940000 AND 964999 AND id NOT IN (SELECT item_id FROM dynamic_items) AND itemtype <> 54)
UNION ALL
SELECT 'spells_new', COUNT(*) FROM spells_new WHERE id BETWEEN 50001 AND 50024
UNION ALL
SELECT 'npc_types', COUNT(*) FROM npc_types
 WHERE id BETWEEN 1200000 AND 1200050
   AND name NOT LIKE 'Rift\_%' AND name NOT LIKE 'Chronal\_%' AND name NOT LIKE 'Tempered\_%'
   AND name NOT LIKE 'Exalted\_%' AND name NOT LIKE 'Eternal\_%' AND name NOT LIKE 'Timeless\_%'
UNION ALL
SELECT 'merchantlist', COUNT(*) FROM merchantlist
 WHERE merchantid BETWEEN 1200000 AND 1200050 AND item NOT BETWEEN 900000 AND 964999
UNION ALL
SELECT 'spawngroup', COUNT(*) FROM spawngroup
 WHERE id IN (5003834, 5003835, 5003837) AND name NOT IN ('rift_archetypes', 'rift_controller', 'rift_vendor')
UNION ALL
SELECT 'alternate_currency 2', COUNT(*) FROM alternate_currency WHERE id = 2 AND item_id <> 800002
UNION ALL
SELECT 'spells_new (08)', COUNT(*) FROM spells_new
 WHERE (id IN (36856, 43002, 43005) AND name NOT LIKE 'Echo of %')
    OR (id = 17779 AND name NOT IN ('PC Spell - RESERVED', 'Echo of Luck'))
UNION ALL
SELECT 'npc_types (09)', COUNT(*) FROM npc_types WHERE id = 1200060 AND name <> 'Soulbinder_Ilsara'
UNION ALL
SELECT 'spawngroup (09)', COUNT(*) FROM spawngroup WHERE id = 5003853 AND name <> 'thevoida_soulbinder';
