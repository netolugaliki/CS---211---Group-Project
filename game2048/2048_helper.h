#ifndef CS211_2048_HELPER_H
#define CS211_2048_HELPER_H
#include <random>
#include <string>
constexpr int SIZE = 5;
struct Game {
    int board[SIZE][SIZE] = {};
    long long score = 0;
    long long moves = 0;
};
// Movement is deterministic. These functions never insert random tiles.
bool move_left(int board[][SIZE], long long& gained);
bool move_right(int board[][SIZE], long long& gained);
bool move_up(int board[][SIZE], long long& gained);
bool move_down(int board[][SIZE], long long& gained);
bool insert_random(int board[][SIZE], std::mt19937& rng);
void new_game(Game& game, std::mt19937& rng);
bool play_move(Game& game, char direction, std::mt19937& rng);
int highest_tile(const Game& game);
bool game_over(const Game& game);
void print_board(const Game& game);
bool save_game(const Game& game, const std::string& path, std::string& error);
bool load_game(Game& game, const std::string& path, std::string& error);
#endif
