# Builds the release package for the dynamic items / Greater Rifts add-on:
#   <out>/dynamic-items-code.patch   the server code changes against upstream (common/, world/, zone/)
#   <out>/content/                    docs, generators, rift scripts and the SQL
#   <out>/bin/                        Windows x64 zone, world and shared_memory with their DLLs (--no-bin skips)
#   <out>.zip                         all of the above
#
#   python utils/scripts/dynamic_items/make_package.py [--base <upstream commit>] [--out <dir>] [--no-bin]
#
# Runs from the repository root. Nothing is committed: the patch is made through a temporary git index, and
# includes uncommitted and new files. It's checked against a clean copy of the base before it's written.
import argparse, datetime, os, shutil, subprocess, sys, tarfile, tempfile, io

ap = argparse.ArgumentParser()
ap.add_argument('--base', default=None, help='upstream commit the patch applies to (default: merge-base with master)')
ap.add_argument('--out', default=None, help='output folder (default: build/package/dynamic-items-<date>)')
ap.add_argument('--bin', default='build/bin/RelWithDebInfo', help='folder with the built programs')
ap.add_argument('--no-bin', action='store_true')
args = ap.parse_args()

root = subprocess.run(['git', 'rev-parse', '--show-toplevel'], capture_output=True, text=True, check=True).stdout.strip()
os.chdir(root)


def git(*a, env=None, binary=False):
    r = subprocess.run(['git', *a], capture_output=True, env=env, check=True)
    return r.stdout if binary else r.stdout.decode('utf-8', 'replace').strip()


base = args.base or git('merge-base', 'HEAD', 'master')
stamp = datetime.date.today().strftime('%Y%m%d')
out = os.path.abspath(args.out or os.path.join('build', 'package', f'dynamic-items-{stamp}'))
if os.path.exists(out):
    shutil.rmtree(out)
os.makedirs(out)

CODE_PATHS = ['common', 'world', 'zone', 'utils/patches']
CONTENT = ['docs/dynamic_items', 'utils/scripts/dynamic_items', 'utils/sql/dynamic_items']

# --- Patch: working tree (tracked + new files) against the base, via a throwaway index
with tempfile.TemporaryDirectory() as tmp:
    index = os.path.join(tmp, 'index')
    env = dict(os.environ, GIT_INDEX_FILE=index)
    git('read-tree', 'HEAD', env=env)
    git('add', '-A', '--', *CODE_PATHS, env=env)
    patch = git('diff', '--cached', '--binary', '--full-index', base, '--', *CODE_PATHS, env=env, binary=True)
    files = git('diff', '--cached', '--name-status', base, '--', *CODE_PATHS, env=env).splitlines()

    # Check it applies to a clean copy of the base
    clean = os.path.join(tmp, 'clean')
    os.makedirs(clean)
    archive = git('archive', '--format=tar', base, '--', *CODE_PATHS, binary=True)
    tarfile.open(fileobj=io.BytesIO(archive)).extractall(clean, filter='data')
    patch_path = os.path.join(out, 'dynamic-items-code.patch')
    open(patch_path, 'wb').write(patch)
    check = subprocess.run(['git', 'apply', '--check', '--whitespace=nowarn', patch_path], cwd=clean,
                           capture_output=True, text=True)
    if check.returncode:
        sys.exit(f'patch does not apply to {base[:10]}:\n{check.stderr}')

added = sum(1 for f in files if f.startswith('A'))
print(f'patch: {len(files)} files ({added} new, {len(files) - added} changed) against {base[:10]}, applies cleanly')

# --- Content
for path in CONTENT:
    shutil.copytree(path, os.path.join(out, 'content', path), ignore=shutil.ignore_patterns('__pycache__'))

# --- Binaries
if not args.no_bin:
    bin_out = os.path.join(out, 'bin')
    os.makedirs(bin_out)
    for name in ['zone', 'world', 'shared_memory']:
        for ext in ('.exe', '.pdb'):
            shutil.copy2(os.path.join(args.bin, name + ext), bin_out)
    for f in os.listdir(args.bin):
        if f.lower().endswith('.dll'):
            shutil.copy2(os.path.join(args.bin, f), bin_out)

version = open('common/version.h').read()
db_version = next(l.split()[-1] for l in version.splitlines() if 'CURRENT_BINARY_DATABASE_VERSION' in l)
open(os.path.join(out, 'README.txt'), 'w', newline='\r\n').write(f'''Dynamic Items and Greater Rifts add-on for EQEmu ({stamp})

Built against upstream EQEmu {base[:10]} (database version {db_version}).
Install steps: content/docs/dynamic_items/INSTALL.md

dynamic-items-code.patch  Apply to your source tree with "git apply dynamic-items-code.patch", then build.
                          Merge by hand where your own code changed the same lines.
bin/                      Windows x64 zone, world and shared_memory for that exact upstream version,
                          built with Lua (LuaJIT) and Perl 5.26. perl526.dll is included when it was next to the
                          build; Perl plugins also need Perl 5.26's modules (e.g. Strawberry Perl 5.26). Replace your programs with
                          these only if you run that upstream version without code changes of your own.
content/                  SQL (run in order), generators, rift scripts and docs.
''')

zip_path = shutil.make_archive(out, 'zip', os.path.dirname(out), os.path.basename(out))
print(f'package: {out}\nzip:     {zip_path} ({os.path.getsize(zip_path) / 1024 / 1024:.1f} MB)')
