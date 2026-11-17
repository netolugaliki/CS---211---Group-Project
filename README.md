# CS211 Project: 2048 and Luhn

I completed this project individually with my instructor's approval.
It includes a 5x5 2048 game and a separate Luhn validator, both written
in C++. I also included my design report and test results.

I used the supplied starter materials and ChatGPT/Codex assistance,
as documented in [contributions.md](contributions.md).

- Student: Neto Lugaliki Kilaho
- Participation: solo, approved by my instructor
- Session: MW
- Submission identifier: Neto_Kilaho
- GitHub username: netolugaliki
- Repository URL: https://github.com/netolugaliki/CS---211---Group-Project

## Programs

### 2048

The 2048 game uses a fixed 5x5 integer array. It starts with two tiles
valued at 2. Players move the tiles up, down, left, or right to combine
matching tiles.

Each tile can merge only once per move. A move that changes the board
increases the move count and adds a new tile valued at 2 in a random
empty cell. The value of each merged tile is added to the score.

Players can continue playing after reaching 2048. The game ends when
the board is full and no horizontally or vertically adjacent tiles
can merge.

### Luhn

The Luhn program checks numbers containing exactly 16 ASCII digits.
It keeps leading zeros and reports invalid input separately from
a failed checksum.

Passing the checksum only means the number meets the Luhn check.
It does not confirm that a card exists or can be used for payment.

## Build and run

A C++17-compatible `g++` compiler must be installed and available
on `PATH`.

Run these commands from the project folder in a Linux/macOS terminal
or Windows Git Bash with `g++` installed:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic game2048/main.cpp game2048/2048_helper.cpp -o game2048_app
g++ -std=c++17 -Wall -Wextra -pedantic luhn/main.cpp luhn/luhn.cpp -o luhn_app
./game2048_app
./luhn_app
```

For Windows PowerShell with `g++` installed:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic game2048/main.cpp game2048/2048_helper.cpp -o game2048_app.exe
g++ -std=c++17 -Wall -Wextra -pedantic luhn/main.cpp luhn/luhn.cpp -o luhn_app.exe
.\game2048_app.exe
.\luhn_app.exe
```

The programs were tested on Linux with `g++`. Windows execution
has not been tested.

## Controls

### 2048

Enter one lowercase command per line without extra spaces.

| Command | Action |
|---|---|
| `u` | Move up |
| `d` | Move down |
| `l` | Move left |
| `r` | Move right |
| `s` | Save to `savegame.txt`, replacing the previous save |
| `o` | Load `savegame.txt` |
| `n` | Start a new game; enter exactly `y` to confirm |
| `h` | Show help |
| `e` | Exit without automatically saving |

When starting a new game, any response other than `y` cancels
the reset.

End of input also exits the program, including during the new-game
confirmation prompt.

After game over, movement is disabled, but the other commands still
work. A winning message appears once per game or load when the
highest tile reaches at least 2048.

### Luhn

Enter exactly 16 digits to check a number. The program accepts another
number after displaying each result. Enter `e` to exit.

## Saving and loading

The game saves to `savegame.txt` in the **current working directory**.
This is the directory from which the program runs, which may be
different from the directory containing the executable.

To try the sample save, copy `game2048/sample_save.txt` to
`savegame.txt` in the working directory, then enter `o` in the game.

Linux/macOS/Git Bash:

```sh
cp game2048/sample_save.txt savegame.txt
```

Windows PowerShell:

```powershell
Copy-Item game2048/sample_save.txt savegame.txt
```

### Save file format

The save file contains:

- The marker `CS211_2048_V1`.
- The score and move count, both nonnegative.
- 25 board values arranged in five rows.

Values are separated by whitespace. Each board cell must contain
zero or a power of two greater than or equal to 2.

The program rejects files with missing data, invalid tiles, values
outside the supported range, or extra non-whitespace content.
It checks the data using a temporary `Game` before replacing the
current game, so a failed load does not partially change the board.

Loading restores the saved board, score, and move count without
adding a new tile. The random generator state is not saved.

The program reports errors when opening, writing, or closing a save
file. Saving overwrites the previous file directly. A disk error
during saving may leave an incomplete file, and no backup is created.

## Tests and documentation

The project includes the following documents:

- [report.pdf](report.pdf): program design, dry runs for both programs,
  and Luhn diagrams.
- [tests.md](tests.md): recorded test results, including full board
  values with rows separated by semicolons.
- [contributions.md](contributions.md): my responsibilities and the
  assistance used.

The deterministic tests use a fixed random seed so their results
can be reproduced. Normal gameplay uses a randomly seeded generator.

### Running the tests

Run the unit tests from a separate working directory to keep their
save files out of the project root.

Linux/macOS/Git Bash:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic testing/unit_tests.cpp game2048/2048_helper.cpp luhn/luhn.cpp -o unit_tests
mkdir -p test_run
cd test_run
../unit_tests
cd ..
python3 testing/cli_tests.py
```

Windows PowerShell:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic testing/unit_tests.cpp game2048/2048_helper.cpp luhn/luhn.cpp -o unit_tests.exe
New-Item -ItemType Directory -Force test_run
Set-Location test_run
..\unit_tests.exe
Set-Location ..
python testing/cli_tests.py
```

The Python test runner requires Python 3 and `g++`. It creates its
own temporary directory. Use `python3` or `python`, depending on
your installation.

The Python test runner was tested on Linux. Its Windows compatibility
has not been verified.

## Limits

Board cells use `int`, while the score and move count use `long long`.

On the compiler used for testing:

- The largest accepted tile is `1073741824`.
- The largest score or move count is `9223372036854775807`.

A move that would overflow either type is canceled without changing
the game.

The save validator checks the file format and allowed value ranges.
It does not check whether the saved board, score, and move count
could have resulted from actual gameplay.

## Contributions and submission

My instructor approved me to complete this project individually.
I am the only student contributor. My responsibilities and the
assistance I used are documented in `contributions.md`.

I am responsible for understanding and explaining both programs,
their dry runs, and their test results.