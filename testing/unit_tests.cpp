#include "../game2048/2048_helper.h"
#include "../luhn/luhn.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

int failures = 0;
std::string board_text(const Game& game) {
    std::ostringstream out;
    for (int r = 0; r < SIZE; ++r) {
        if (r) out << "; ";
        for (int c = 0; c < SIZE; ++c) out << (c ? " " : "") << game.board[r][c];
    }
    return out.str();
}
bool same(const Game& a, const Game& b) {
    return board_text(a) == board_text(b) && a.score == b.score && a.moves == b.moves;
}
void check(const std::string& name, const std::string& input,
           const std::string& expected, const std::string& actual, bool pass) {
    std::cout << "| " << name << " | " << input << " | " << expected << " | " << actual
              << " | " << (pass ? "PASS" : "FAIL") << " |\n";
    if (!pass) ++failures;
}
int sum(const Game& g) {
    int total = 0;
    for (const auto& row : g.board) for (int value : row) total += value;
    return total;
}
std::string serialized(const Game& g) {
    std::ostringstream out;
    out << "CS211_2048_V1\n" << g.score << ' ' << g.moves << '\n';
    for (const auto& row : g.board) { for (int v : row) out << v << ' '; out << '\n'; }
    return out.str();
}
int main() {
    std::cout << "| Test | Starting state/input | Expected | Actual | Result |\n"
              << "|---|---|---|---|---|\n";
    const int inputs[][5] = {{0,2,0,2,0},{2,2,2,0,0},{2,2,2,2,0},{2,2,2,2,2},
                            {2,2,4,0,0},{4,4,8,8,0},{2,4,8,0,0}};
    const int outputs[][5] = {{4,0,0,0,0},{4,2,0,0,0},{4,4,0,0,0},{4,4,2,0,0},
                             {4,4,0,0,0},{8,16,0,0,0},{2,4,8,0,0}};
    const int scores[] = {4,4,8,8,4,24,0};
    for (int test = 0; test < 7; ++test) {
        Game g, expected;
        for (int c = 0; c < 5; ++c) { g.board[0][c] = inputs[test][c]; expected.board[0][c] = outputs[test][c]; }
        const std::string before = board_text(g);
        long long gain;
        const bool changed = move_left(g.board, gain);
        check("Left example " + std::to_string(test+1), before,
              board_text(expected) + "; gain " + std::to_string(scores[test]),
              board_text(g) + "; gain " + std::to_string(gain),
              board_text(g) == board_text(expected) && gain == scores[test] && changed == (test != 6));
    }
    for (char direction : std::string("rud")) {
        Game g, expected;
        const int values[5] = {0,2,0,2,2};
        for (int i = 0; i < 5; ++i) {
            if (direction == 'r') g.board[0][i] = values[i];
            else g.board[i][0] = values[i];
        }
        if (direction == 'r') { expected.board[0][3]=2; expected.board[0][4]=4; }
        if (direction == 'u') { expected.board[0][0]=4; expected.board[1][0]=2; }
        if (direction == 'd') { expected.board[3][0]=2; expected.board[4][0]=4; }
        std::string before = board_text(g);
        long long gain;
        if (direction == 'r') move_right(g.board,gain);
        if (direction == 'u') move_up(g.board,gain);
        if (direction == 'd') move_down(g.board,gain);
        check(std::string("Direction ")+direction, before, board_text(expected)+"; gain 4",
              board_text(g)+"; gain "+std::to_string(gain), board_text(g)==board_text(expected) && gain==4);
    }
    std::mt19937 rng(211);
    Game g;
    new_game(g,rng);
    int count = 0;
    for (const auto& row : g.board) for (int v : row) if (v == 2) ++count;
    check("New game", "fresh state", "two distinct 2s; score/moves 0", board_text(g), count==2 && sum(g)==4 && g.score==0 && g.moves==0);
    g = Game{}; g.board[0][2]=4; g.score=8; g.moves=3;
    const Game slideBefore = g;
    Game deterministic = g;
    long long gain;
    move_left(deterministic.board,gain);
    bool changed = play_move(g,'l',rng);
    int inserted = 0; bool exact = true;
    for (int r=0;r<5;++r) for (int c=0;c<5;++c) {
        if (g.board[r][c] != deterministic.board[r][c]) {
            ++inserted;
            if (deterministic.board[r][c]!=0 || g.board[r][c]!=2) exact=false;
        }
    }
    check("Slide then insertion",board_text(slideBefore)+"; score 8, moves 3",
          "before insertion: "+board_text(deterministic)+"; afterward exactly one new 2, score 8, moves 4",
          board_text(g)+"; score "+std::to_string(g.score)+", moves "+std::to_string(g.moves),
          changed && exact && inserted==1 && gain==0 && g.score==8 && g.moves==4);
    g = Game{}; g.board[0][0]=2; g.board[0][1]=4; g.score=12; g.moves=7;
    Game before=g; auto beforeRng=rng;
    changed=play_move(g,'l',rng);
    check("Unchanged move",board_text(before)+"; score 12, moves 7","identical board/counters and RNG",board_text(g),!changed && same(g,before) && rng==beforeRng);
    for(int r=0;r<5;++r) for(int c=0;c<5;++c) g.board[r][c]=(r+c)%2 ? 4 : 2;
    before=g;
    changed=play_move(g,'r',rng);
    check("Full stuck board",board_text(before),"game over; move rejected; state preserved",board_text(g),game_over(g) && !changed && same(g,before));
    g.board[0][1]=2;
    before=g;
    bool playable=!game_over(g);
    changed=play_move(g,'l',rng);
    check("Full board with merge",board_text(before),"not game over; move accepted; score +4, moves +1, tile sum +2",board_text(g),playable && changed && g.score==before.score+4 && g.moves==before.moves+1 && sum(g)==sum(before)+2);
    g=Game{}; g.board[0][0]=1024; g.board[0][1]=1024; g.board[1][0]=4; g.board[1][1]=4;
    before=g;
    changed=play_move(g,'l',rng);
    check("Multiple merges / win",board_text(before),"2048 and 8 merged; score 2056, moves 1; one new 2",board_text(g)+"; score "+std::to_string(g.score),changed && highest_tile(g)==2048 && g.score==2056 && g.moves==1 && sum(g)==sum(before)+2);
    const Game saved=g;
    std::string error;
    bool written=save_game(g,"roundtrip.txt",error);
    new_game(g,rng);
    bool loaded=load_game(g,"roundtrip.txt",error);
    check("Save/change/load",board_text(saved)+"; score 2056, moves 1","exact restoration",board_text(g),written && loaded && same(g,saved));
    const std::string good=serialized(saved);
    const std::string badFiles[] = {
        "", "WRONG\n0 0\n", "CS211_2048_V1\n-1 0\n", "CS211_2048_V1\n0 -1\n",
        "CS211_2048_V1\n99999999999999999999999 0\n", "CS211_2048_V1\n0 0\n2 4",
        "CS211_2048_V1\n0 0\n3 ", "CS211_2048_V1\n0 0\n1 ",
        "CS211_2048_V1\n0 0\n-2 ", "CS211_2048_V1\n0 0\n2147483648 ",
        good+" 2", good+" junk", good.substr(0,good.size()-2)+"0.5"
    };
    const char* labels[]={"empty","bad marker","negative score","negative moves","counter overflow","incomplete board","tile 3","tile 1","negative tile","tile overflow","extra integer","extra text","fractional final cell"};
    for(int i=0;i<13;++i) {
        std::ofstream out("invalid.txt"); out << badFiles[i]; out.close();
        before=g;
        loaded=load_game(g,"invalid.txt",error);
        check(std::string("Invalid load: ")+labels[i],"saved winning board active; "+std::string(labels[i]),"reject; preserve all cells/counters",error+"; preserved="+(same(g,before)?"yes":"no"),!loaded && same(g,before));
    }
    before=g; loaded=load_game(g,"missing-folder/save.txt",error);
    check("Missing load",board_text(before),"reject; preserve state",error,!loaded && same(g,before));
    bool ok=save_game(g,"missing-folder/save.txt",error);
    check("Save open failure","nonexistent parent directory","report failure; preserve state",error,!ok && same(g,before));
    std::ofstream whitespace("whitespace.txt"); whitespace << good << "\n \t\n"; whitespace.close();
    loaded=load_game(g,"whitespace.txt",error);
    check("Trailing whitespace","valid save plus whitespace","accept exact state",loaded?"accepted":"rejected",loaded && same(g,saved));
    g=Game{}; g.board[0][0]=1073741824; g.board[0][1]=1073741824; before=g;
    bool threw=false; try {play_move(g,'l',rng);} catch(const std::overflow_error&) {threw=true;}
    check("Tile overflow guard",board_text(before),"cancel without mutation",threw?"exception; state preserved":"no exception",threw && same(g,before));
    g=Game{}; g.board[0][1]=2; g.moves=std::numeric_limits<long long>::max(); before=g;
    threw=false; try {play_move(g,'l',rng);} catch(const std::overflow_error&) {threw=true;}
    check("Move counter limit","moves at maximum; slide possible","cancel without mutation",threw?"exception; state preserved":"no exception",threw && same(g,before));
    g=Game{}; g.board[0][0]=2; g.board[0][1]=2; g.score=std::numeric_limits<long long>::max(); before=g;
    threw=false; try {play_move(g,'l',rng);} catch(const std::overflow_error&) {threw=true;}
    check("Score counter limit","score at maximum; merge possible","cancel without mutation",threw?"exception; state preserved":"no exception",threw && same(g,before));
    const std::string numbers[]={"4532015112830366","4532015112830367","0000000000000018","453201511283036","45320151128303666","453201511283036X","45320151 2830366","","453201511283036!"," 4532015112830366"};
    for(int i=0;i<10;++i) {
        const bool format=i<3, pass=i==0 || i==2;
        const int expectedSum=i==0?50:i==1?51:i==2?10:-1;
        check("Luhn "+std::to_string(i+1),numbers[i].empty()?"empty line":numbers[i],
              format?(pass?"format valid; passes":"format valid; fails"):"malformed; sum -1; false",
              "format="+std::to_string(validInput(numbers[i]))+", sum="+std::to_string(luhnSum(numbers[i]))+", checksum="+std::to_string(checkLuhn(numbers[i])),
              validInput(numbers[i])==format && checkLuhn(numbers[i])==pass && luhnSum(numbers[i])==expectedSum);
    }
    return failures ? 1 : 0;
}
