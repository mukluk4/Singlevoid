# Database access for the dynamic items / rift generators.
#
# connect(source) returns query(sql) -> list of rows (each a list of strings, NULL as 'NULL'), like `mysql -N -B`.
#   source = path to eqemu_config.json   -> runs the SQL on that server's database with the mysql client
#                                            (MYSQL_EXE overrides the client path; the password goes in MYSQL_PWD)
#   source = path to a mysqldump .sql    -> loads the tables each query names into an in-memory SQLite copy
#                                            (e.g. PEQ's create_tables_content.sql), so the SQL can be generated
#                                            for a stock database without one running. Nothing is written anywhere.
# Generated SQL is plain MySQL either way; only reading differs.
import json, os, re, shutil, sqlite3, subprocess

DEFAULT_MYSQL = [r'C:\Program Files\MariaDB 12.3\bin\mysql.exe', 'mysql']


def connect(source):
    if source.lower().endswith('.json'):
        return _live(source)
    return _Dump(source).query


def _live(config_path):
    cfg = json.load(open(config_path))['server']['database']
    env = dict(os.environ, MYSQL_PWD=cfg['password'])
    mysql = os.environ.get('MYSQL_EXE') or next((m for m in DEFAULT_MYSQL if os.path.exists(m) or shutil.which(m)), 'mysql')

    def query(sql):
        out = subprocess.run([mysql, '-h', cfg['host'], '-P', str(cfg['port']), '-u', cfg['username'], cfg['db'],
                              '-N', '-B', '-e', sql], env=env, capture_output=True, text=True, check=True).stdout
        return [line.split('\t') for line in out.splitlines()]
    return query


class _Dump:
    """The tables a query names, loaded on first use from a mysqldump file into SQLite."""

    COLUMNS_QUERY = re.compile(r"information_schema\.columns\s+WHERE.*table_name\s*=\s*'(\w+)'", re.I | re.S)

    def __init__(self, path):
        self.path = path
        self.db = sqlite3.connect(':memory:')
        self.db.create_function('REGEXP', 2, lambda pattern, value: value is not None and re.search(pattern, str(value)) is not None)
        self.columns = {}
        self.loaded = set()
        self._scan_schema()

    def _scan_schema(self):
        create, table = [], None
        with open(self.path, encoding='utf-8', errors='replace') as fh:
            for line in fh:
                m = re.match(r'CREATE TABLE `(\w+)`', line)
                if m:
                    table, create = m.group(1), []
                    continue
                if table:
                    if line.startswith(')'):
                        self.columns[table] = [re.match(r'\s*`(\w+)`', l).group(1) for l in create if re.match(r'\s*`\w+`', l)]
                        table = None
                    else:
                        create.append(line)

    def _load(self, tables):
        tables = [t for t in tables if t in self.columns and t not in self.loaded]
        if not tables:
            return
        for t in tables:
            cols = ', '.join(f'"{c}"' for c in self.columns[t])
            self.db.execute(f'CREATE TABLE "{t}" ({cols})')
        wanted, current, buf = set(tables), None, []
        with open(self.path, encoding='utf-8', errors='replace') as fh:
            for line in fh:
                if current is None:
                    m = re.match(r'INSERT INTO `(\w+)`(?: \([^)]*\))? VALUES(.*)', line, re.S)
                    if m and m.group(1) in wanted:
                        current, buf = m.group(1), [m.group(2)]
                        if line.rstrip().endswith(';'):
                            self._insert(current, ''.join(buf))
                            current = None
                    continue
                buf.append(line)
                if line.rstrip().endswith(';'):
                    self._insert(current, ''.join(buf))
                    current = None
        self.loaded.update(tables)

    def _insert(self, table, values):
        rows = _parse_values(values)
        marks = ', '.join('?' * len(self.columns[table]))
        self.db.executemany(f'INSERT INTO "{table}" VALUES ({marks})', rows)

    def query(self, sql):
        m = self.COLUMNS_QUERY.search(sql)
        if m:
            return [[c] for c in self.columns.get(m.group(1), [])]
        self._load(set(re.findall(r'\b(?:FROM|JOIN)\s+`?(\w+)`?', sql, re.I)))
        # MySQL-only syntax used by the generators
        lite = re.sub(r'\bNOT\s+REGEXP\b', 'NOT REGEXP', sql, flags=re.I)
        out = []
        for row in self.db.execute(lite):
            out.append(['NULL' if v is None else _fmt(v) for v in row])
        return out


def _fmt(v):
    if isinstance(v, float) and v.is_integer():
        return f'{v:.6f}'
    return str(v)


def _parse_values(s):
    """mysqldump VALUES (...),(...) -> rows; quoted values stay text, bare numbers become numbers."""
    rows, i, n = [], 0, len(s)
    while i < n:
        if s[i] != '(':
            i += 1
            continue
        i += 1
        row = []
        while True:
            if s[i] == "'":
                i += 1
                buf = []
                while True:
                    c = s[i]
                    if c == '\\':
                        buf.append({'n': '\n', 'r': '\r', 't': '\t', '0': '\0'}.get(s[i + 1], s[i + 1]))
                        i += 2
                        continue
                    if c == "'":
                        if i + 1 < n and s[i + 1] == "'":
                            buf.append("'")
                            i += 2
                            continue
                        i += 1
                        break
                    buf.append(c)
                    i += 1
                row.append(''.join(buf))
            else:
                j = i
                while s[j] not in ',)':
                    j += 1
                tok = s[i:j].strip()
                if tok == 'NULL':
                    row.append(None)
                else:
                    try:
                        row.append(int(tok))
                    except ValueError:
                        row.append(float(tok))
                i = j
            if s[i] == ',':
                i += 1
                continue
            i += 1
            break
        rows.append(row)
    return rows
