"""Integration tests. Requires Python 3 and g++ on PATH; uses temporary files only."""
import pathlib
import re
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
rows = []

def check(name, inputs, expected, actual, passed):
    rows.append((name, inputs, expected, actual, 'PASS' if passed else 'FAIL'))
    if not passed:
        raise AssertionError((name, actual))

with tempfile.TemporaryDirectory() as folder:
    work = pathlib.Path(folder)
    for name, sources in [('game', ['game2048/main.cpp', 'game2048/2048_helper.cpp']),
                          ('luhn', ['luhn/main.cpp', 'luhn/luhn.cpp'])]:
        subprocess.run(['g++', '-std=c++17', '-Wall', '-Wextra', '-pedantic',
                        *[str(ROOT / f) for f in sources], '-o', str(work / name)], check=True)
    def run(name, inputs):
        return subprocess.run([str(work / name)], input=inputs, text=True,
                              capture_output=True, cwd=work, check=True, timeout=5).stdout
    def snapshots(output):
        return re.findall(r'Score: [^\n]+\n(?:\|[^\n]+\n){5}', output)
    out = run('game', 's\nh\nxyz\nn\nno\ns\no\ne\n')
    boards = snapshots(out)
    check('Help, invalid command, canceled new game, save, load', 's h xyz n no s o e',
          'state unchanged; useful messages; exit', 'identical startup/loaded boards; expected messages',
          len(boards)==2 and boards[0]==boards[1] and 'Invalid command.' in out and 'New game canceled.' in out and out.endswith('Goodbye.\n'))
    check('Exit has no extra turn', 'e', 'one startup board; Goodbye', 'one board; Goodbye',
          len(snapshots(run('game','e\n')))==1)
    out=run('game','n\ny\ne\n')
    boards=snapshots(out)
    check('Confirmed new game', 'n y e', 'two 2s and reset counters', 'two tiles; score/moves zero',
          len(boards)==2 and 'Score: 0 | Successful moves: 0 | Highest tile: 2' in boards[1]
          and sum(int(v) for v in re.findall(r'\b\d+\b','\n'.join(boards[1].splitlines()[1:])))==4)
    for inputs in ['', 'n\n']:
        out=run('game',inputs)
        check('Game EOF '+repr(inputs),repr(inputs),'clean Goodbye','Goodbye',out.endswith('Goodbye.\n'))
    stuck=[[2 if (r+c)%2==0 else 4 for c in range(5)] for r in range(5)]
    def save(board,score=0,moves=0):
        (work/'savegame.txt').write_text('CS211_2048_V1\n'+f'{score} {moves}\n'+'\n'.join(' '.join(map(str,row)) for row in board)+'\n')
    save(stuck)
    out=run('game','o\nl\ns\nh\ne\n')
    boards=snapshots(out)
    check('Game-over controls','load checkerboard; l s h e','movement blocked; state preserved; save/help usable',
          'movement disabled; loaded and final boards identical; saved',
          'movement is disabled.' in out and 'Game saved.' in out and boards[1]==boards[2])
    winning=[[1024,1024,0,0,0]]+[[0]*5 for _ in range(4)]
    save(winning)
    out=run('game','o\nl\nr\ne\n')
    check('Win and continued play','load [1024 1024 0 0 0]; l r e','announce success once; second move accepted',
          'one success; moves reaches 2',out.count('Success!')==1 and 'Successful moves: 2' in out)
    (work/'savegame.txt').write_text('bad save')
    out=run('game','o\ne\n')
    check('Malformed load message','o with invalid save','Game unchanged message','Game unchanged', 'Game unchanged.' in out)
    out=run('luhn','4532015112830366\n4532015112830367\n0000000000000018\n45320151 2830366\ne\n')
    check('Repeated Luhn interaction','valid, fail, leading zeros, internal space, e',
          '2 passes, 1 checksum failure, 1 format error; exit','2 passes, 1 failure, 1 format error; Goodbye',
          out.count('Checksum PASSES')==2 and out.count('Checksum FAILS')==1 and out.count('Invalid format')==1 and out.endswith('Goodbye.\n'))
    out=run('luhn','')
    check('Luhn EOF','empty stream','clean Goodbye','Goodbye',out.endswith('Goodbye.\n'))
print('| Test | Input/action | Expected | Actual | Result |')
print('|---|---|---|---|---|')
for row in rows:
    print('| '+' | '.join(row)+' |')
