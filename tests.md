# Test results for 2048 and Luhn

Neto Lugaliki Kilaho | CS211 | MW | Solo project

All 56 automated checks passed in the November 17, 2026 Linux test run. Both programs and the C++ test driver compiled with g++ 14.2.0 using `-std=c++17 -Wall -Wextra -pedantic` without warnings or errors.

The C++ tests are in `testing/unit_tests.cpp`, and the console tests are in `testing/cli_tests.py`. The unit tests use a fixed random seed of `211` so the results can be reproduced. Normal gameplay seeds the generator with `std::random_device`.

## Deterministic functions and persistence

Board notation: five rows separated by semicolons; entries in each row are space-separated. Direction tests show the board BEFORE random insertion.
The separate insertion test compares every cell with that deterministic result, requiring exactly one formerly empty cell to become 2 and no other differences.

Invalid save tests begin from the same saved winning board shown in the round-trip row and compare all 25 cells and both counters after rejection.

| Test | Starting state/input | Expected | Actual | Result |
|---|---|---|---|---|
| Left example 1 | 0 2 0 2 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 4 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | 4 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | PASS |
| Left example 2 | 2 2 2 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 4 2 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | 4 2 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | PASS |
| Left: four equal tiles form two pairs | 2 2 2 2 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 4 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 8 | 4 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 8 | PASS |
| Left example 4 | 2 2 2 2 2; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 4 4 2 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 8 | 4 4 2 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 8 | PASS |
| Left: a tile can merge only once per move | 2 2 4 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 4 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | 4 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | PASS |
| Left example 6 | 4 4 8 8 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 8 16 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 24 | 8 16 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 24 | PASS |
| Left: a packed row with no matching tiles stays unchanged | 2 4 8 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 2 4 8 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 0 | 2 4 8 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 0 | PASS |
| Direction r | 0 2 0 2 2; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 0 0 0 2 4; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | 0 0 0 2 4; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | PASS |
| Direction u | 0 0 0 0 0; 2 0 0 0 0; 0 0 0 0 0; 2 0 0 0 0; 2 0 0 0 0 | 4 0 0 0 0; 2 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | 4 0 0 0 0; 2 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; gain 4 | PASS |
| Direction d | 0 0 0 0 0; 2 0 0 0 0; 0 0 0 0 0; 2 0 0 0 0; 2 0 0 0 0 | 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 2 0 0 0 0; 4 0 0 0 0; gain 4 | 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 2 0 0 0 0; 4 0 0 0 0; gain 4 | PASS |
| New game | fresh state | two distinct 2s; score/moves 0 | 0 0 0 2 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 2; 0 0 0 0 0 | PASS |
| Slide then insertion | 0 0 4 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; score 8, moves 3 | before insertion: 4 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; afterward exactly one new 2, score 8, moves 4 | 4 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 2 0 0; 0 0 0 0 0; score 8, moves 4 | PASS |
| Unchanged move | 2 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; score 12, moves 7 | identical board/counters and RNG | 2 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | PASS |
| Full stuck board | 2 4 2 4 2; 4 2 4 2 4; 2 4 2 4 2; 4 2 4 2 4; 2 4 2 4 2 | game over; move rejected; state preserved | 2 4 2 4 2; 4 2 4 2 4; 2 4 2 4 2; 4 2 4 2 4; 2 4 2 4 2 | PASS |
| Full board with merge | 2 2 2 4 2; 4 2 4 2 4; 2 4 2 4 2; 4 2 4 2 4; 2 4 2 4 2 | not game over; move accepted; score +4, moves +1, tile sum +2 | 4 2 4 2 2; 4 2 4 2 4; 2 4 2 4 2; 4 2 4 2 4; 2 4 2 4 2 | PASS |
| Multiple merges / win | 1024 1024 0 0 0; 4 4 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | 2048 and 8 merged; score 2056, moves 1; one new 2 | 2048 0 0 0 0; 8 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 2; score 2056 | PASS |
| Save/change/load | 2048 0 0 0 0; 8 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 2; score 2056, moves 1 | exact restoration | 2048 0 0 0 0; 8 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 2 | PASS |
| Invalid load: empty | saved winning board active; empty | reject; preserve all cells/counters | Invalid save version marker.; preserved=yes | PASS |
| Invalid load: bad marker | saved winning board active; bad marker | reject; preserve all cells/counters | Invalid save version marker.; preserved=yes | PASS |
| Invalid load: negative score | saved winning board active; negative score | reject; preserve all cells/counters | Invalid, missing, or out-of-range counters.; preserved=yes | PASS |
| Invalid load: negative moves | saved winning board active; negative moves | reject; preserve all cells/counters | Invalid, missing, or out-of-range counters.; preserved=yes | PASS |
| Invalid load: counter overflow | saved winning board active; counter overflow | reject; preserve all cells/counters | Invalid, missing, or out-of-range counters.; preserved=yes | PASS |
| Invalid load: incomplete board | saved winning board active; incomplete board | reject; preserve all cells/counters | Invalid, missing, or out-of-range board value.; preserved=yes | PASS |
| Invalid load: tile 3 | saved winning board active; tile 3 | reject; preserve all cells/counters | Invalid, missing, or out-of-range board value.; preserved=yes | PASS |
| Invalid load: tile 1 | saved winning board active; tile 1 | reject; preserve all cells/counters | Invalid, missing, or out-of-range board value.; preserved=yes | PASS |
| Invalid load: negative tile | saved winning board active; negative tile | reject; preserve all cells/counters | Invalid, missing, or out-of-range board value.; preserved=yes | PASS |
| Invalid load: tile overflow | saved winning board active; tile overflow | reject; preserve all cells/counters | Invalid, missing, or out-of-range board value.; preserved=yes | PASS |
| Invalid load: extra integer | saved winning board active; extra integer | reject; preserve all cells/counters | Extra content or read error in save file.; preserved=yes | PASS |
| Invalid load: extra text | saved winning board active; extra text | reject; preserve all cells/counters | Extra content or read error in save file.; preserved=yes | PASS |
| Invalid load: fractional final cell | saved winning board active; fractional final cell | reject; preserve all cells/counters | Invalid, missing, or out-of-range board value.; preserved=yes | PASS |
| Missing load | 2048 0 0 0 0; 8 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 2 | reject; preserve state | Cannot open save file for reading. | PASS |
| Save open failure | nonexistent parent directory | report failure; preserve state | Cannot open save file for writing. | PASS |
| Trailing whitespace | valid save plus whitespace | accept exact state | accepted | PASS |
| Tile overflow guard | 1073741824 1073741824 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0; 0 0 0 0 0 | cancel without mutation | exception; state preserved | PASS |
| Move counter limit | moves at maximum; slide possible | cancel without mutation | exception; state preserved | PASS |
| Score counter limit | score at maximum; merge possible | cancel without mutation | exception; state preserved | PASS |
| Luhn 1 | 4532015112830366 | format valid; passes | format=1, sum=50, checksum=1 | PASS |
| Luhn 2 | 4532015112830367 | format valid; fails | format=1, sum=51, checksum=0 | PASS |
| Luhn 3 | 0000000000000018 | format valid; passes | format=1, sum=10, checksum=1 | PASS |
| Luhn 4 | 453201511283036 | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |
| Luhn 5 | 45320151128303666 | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |
| Luhn 6 | 453201511283036X | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |
| Luhn 7 | 45320151 2830366 | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |
| Luhn 8 | empty line | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |
| Luhn 9 | 453201511283036! | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |
| Luhn 10 |  4532015112830366 | malformed; sum -1; false | format=0, sum=-1, checksum=0 | PASS |

## Console integration

| Test | Input/action | Expected | Actual | Result |
|---|---|---|---|---|
| Help, invalid command, canceled new game, save, load | s h xyz n no s o e | state unchanged; useful messages; exit | identical startup/loaded boards; expected messages | PASS |
| Exit has no extra turn | e | one startup board; Goodbye | one board; Goodbye | PASS |
| Confirmed new game | n y e | two 2s and reset counters | two tiles; score/moves zero | PASS |
| Game EOF '' | '' | clean Goodbye | Goodbye | PASS |
| Game EOF 'n\n' | 'n\n' | clean Goodbye | Goodbye | PASS |
| Game-over controls | load checkerboard; l s h e | movement blocked; state preserved; save/help usable | movement disabled; loaded and final boards identical; saved | PASS |
| Win and continued play | load [1024 1024 0 0 0]; l r e | announce success once; second move accepted | one success; moves reaches 2 | PASS |
| Malformed load message | o with invalid save | Game unchanged message | Game unchanged | PASS |
| Repeated Luhn interaction | valid, fail, leading zeros, internal space, e | 2 passes, 1 checksum failure, 1 format error; exit | 2 passes, 1 failure, 1 format error; Goodbye | PASS |
| Luhn EOF | empty stream | clean Goodbye | Goodbye | PASS |

## Build check

Both programs compiled from a fresh copy of the ZIP using the commands in the README. Each program launched and exited successfully on Linux. The Luhn validator reported a passing checksum of 50 for `4532015112830366`. The unit and console test results from the extracted copy matched the tables above.
