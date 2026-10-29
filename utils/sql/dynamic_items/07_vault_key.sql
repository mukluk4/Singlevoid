-- Nautilus Vault key (item 9011009), only needed with rule Custom:NautilusVault on. Run after 01_rift_base.sql.
-- Right-clicking it opens the vault window of the NMS client add-on (dinput8.dll). The zone gives it to
-- every character on zone-in while the rule is on. clickeffect 1 is a client dummy: the zone catches the
-- click and never casts it. nodrop = 0 is No Drop in this schema; norent = 1 keeps it through long camps.
-- Copied from the dynamic item shell (900101) so every items column exists. Safe to run again.

START TRANSACTION;

DROP TEMPORARY TABLE IF EXISTS vault_key_tmp;
CREATE TEMPORARY TABLE vault_key_tmp SELECT * FROM items WHERE id = 900101;
UPDATE vault_key_tmp SET
    id = 9011009,
    Name = 'Nautilus Vault',
    lore = 'Your Nautilus Vault. Right-click to open storage, bank, merchant, Proc Locker.',
    comment = 'Nautilus Vault key (dynamic items add-on)',
    icon = 6306, idfile = 'IT64',
    itemclass = 0, itemtype = 33, slots = 0, classes = 65535, races = 65535,
    bagsize = 0, bagslots = 0, bagtype = 0, bagwr = 0, book = 0,
    nodrop = 0, norent = 1, notransfer = 1, fvnodrop = 1, loregroup = -1, magic = 1,
    weight = 1, size = 1, price = 0, sellrate = 0, stackable = 0, potionbelt = 0, attuneable = 0,
    augslot1type = 0, augslot2type = 0, augslot3type = 0, augslot4type = 0, augslot5type = 0, augslot6type = 0,
    clicktype = 1, clickeffect = 1, clicklevel = 0, clicklevel2 = 0, clickname = 'Open Nautilus Vault',
    casttime = 0, casttime_ = 0, recastdelay = 0, recasttype = 0, maxcharges = -1;
DELETE FROM items WHERE id = 9011009;
INSERT INTO items SELECT * FROM vault_key_tmp;
DROP TEMPORARY TABLE vault_key_tmp;

COMMIT;
