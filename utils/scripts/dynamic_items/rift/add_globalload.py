# Adds the models the rift NPCs use (Guardian looks, the Pylon's Grandfather Clock, the return Portal) to a
# player's EverQuest Resources/GlobalLoad.txt, so they render in every zone. Players without it see the
# default placeholder model for those NPCs; nothing else changes. Lines already there are skipped.
#
#   python add_globalload.py "<EverQuest folder>/Resources/GlobalLoad.txt"      (keeps a .bak copy)
import shutil, sys

MODELS = [
    ('gfc.eqg', 'Rift Pylon (Grandfather Clock)'),
    ('bothunder_chr', 'Rift Portal'),
    ('fkn.eqg', 'Rift Guardian: Fallen Knight'),
    ('spt.eqg', 'Rift Guardian: Spectre'),
    ('bse.eqg', 'Rift Guardian: Banshee'),
    ('ski.eqg', 'Rift Guardian: Iksar Skeleton'),
    ('xim.eqg', 'Rift Guardian: Iksar Ghost'),
    ('drl.eqg', 'Rift Guardian: Dracolich'),
    ('srk.eqg', 'Rift Guardian: Sarnak'),
    ('ddm.eqg', 'Rift Guardian: Dragorn'),
    ('ggy.eqg', 'Rift Guardian: Gargoyle'),
    ('vam.eqg', 'Rift Guardian: Vampire'),
    ('wwf.eqg', 'Rift Guardian: Werewolf'),
    ('cth.eqg', 'Rift Guardian: Cazic Thule'),
    ('amy.eqg', 'Rift Guardian: Amygdalan'),
    ('myg.eqg', 'Rift Guardian: Dark Lord'),
    ('glb.eqg', 'Rift Guardian: Bone Golem'),
    ('frd.eqg', 'Rift Guardian: Gelidran'),
    ('gra.eqg', 'Rift Guardian: Rallosian Giant'),
    ('drg.eqg', 'Rift Guardian: Dragon'),
    ('gua.eqg', 'Rift Guardian: Clockwork Guardian'),
    ('spq.eqg', 'Rift Guardian: Spider Queen'),
    ('iwm_chr', 'Rift Guardian: Knight of Hate'),
    ('izm_chr', 'Rift Guardian: Arcanist of Hate'),
    ('ahm.eqg', 'Rift Guardian: Akheva'),
    ('sos.eqg', 'Rift Guardian: Servant of Shadow'),
    ('alg.eqg', 'Rift Guardian: Alaran Ghost'),
    ('mrt.eqg', 'Rift Guardian: Marionette'),
]

path = sys.argv[1]
raw = open(path, 'rb').read().decode('ascii')
nl = '\r\n' if '\r\n' in raw else '\n'
lines = raw.split(nl)
have = {l.split(',')[3].lower() for l in lines if l.count(',') >= 4}
add = [f'3,0,TFFFC,{f},Loading {label}...' for f, label in MODELS if f.lower() not in have]
if not add:
    sys.exit('already up to date')

# After the last group-3 line (before the Global2_chr / Global3_chr lines)
group3 = [i for i, l in enumerate(lines) if l.startswith('3,')]
at = group3[-1] + 1 if group3 else len(lines)
lines[at:at] = add
shutil.copyfile(path, path + '.bak')
open(path, 'wb').write(nl.join(lines).encode('ascii'))
print(f'added {len(add)} lines ({len(MODELS) - len(add)} already there); backup in {path}.bak')
