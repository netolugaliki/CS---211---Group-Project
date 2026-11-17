#include "2048_helper.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
// Read each row/column from the edge toward which the tiles move.
void coordinates(char direction, int line, int offset, int& row, int& col) {
    if (direction == 'l' || direction == 'r') {
        row = line;
        col = direction == 'l' ? offset : SIZE - 1 - offset;
    } else {
        row = direction == 'u' ? offset : SIZE - 1 - offset;
        col = line;
    }
}
bool move_board(int board[][SIZE], char direction, long long& gained) {
    int result[SIZE][SIZE] = {};
    gained = 0;
    for (int line = 0; line < SIZE; ++line) {
        int compact[SIZE] = {}, merged[SIZE] = {};
        int count = 0, used = 0;
        for (int offset = 0; offset < SIZE; ++offset) {
            int row, col;
            coordinates(direction, line, offset, row, col);
            if (board[row][col] != 0) compact[count++] = board[row][col];
        }
        for (int i = 0; i < count; ++i) {
            if (i + 1 < count && compact[i] == compact[i + 1]) {
                if (compact[i] > std::numeric_limits<int>::max() / 2)
                    throw std::overflow_error("Tile limit reached; move canceled.");
                merged[used] = compact[i] * 2;
                gained += merged[used++];
                ++i; // Consume both original tiles: merged tiles are never revisited.
            } else {
                merged[used++] = compact[i];
            }
        }
        for (int offset = 0; offset < SIZE; ++offset) {
            int row, col;
            coordinates(direction, line, offset, row, col);
            result[row][col] = merged[offset];
        }
    }
    bool changed = false;
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c) {
            if (board[r][c] != result[r][c]) changed = true;
            board[r][c] = result[r][c];
        }
    return changed;
}
bool valid_tile(int value) {
    return value == 0 || (value >= 2 && (value & (value - 1)) == 0);
}
}
bool move_left(int board[][SIZE], long long& gained) { return move_board(board, 'l', gained); }
bool move_right(int board[][SIZE], long long& gained) { return move_board(board, 'r', gained); }
bool move_up(int board[][SIZE], long long& gained) { return move_board(board, 'u', gained); }
bool move_down(int board[][SIZE], long long& gained) { return move_board(board, 'd', gained); }

bool insert_random(int board[][SIZE], std::mt19937& rng) {
    int empty[SIZE * SIZE], count = 0;
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (board[r][c] == 0) empty[count++] = r * SIZE + c;
    if (count == 0) return false;
    std::uniform_int_distribution<int> pick(0, count - 1);
    const int position = empty[pick(rng)];
    board[position / SIZE][position % SIZE] = 2;
    return true;
}
void new_game(Game& game, std::mt19937& rng) {
    game = Game{};
    insert_random(game.board, rng);
    insert_random(game.board, rng);
}
bool play_move(Game& game, char direction, std::mt19937& rng) {
    if (game_over(game)) return false;
    Game next = game;
    long long gained = 0;
    bool changed = false;
    switch (direction) {
        case 'l': changed = move_left(next.board, gained); break;
        case 'r': changed = move_right(next.board, gained); break;
        case 'u': changed = move_up(next.board, gained); break;
        case 'd': changed = move_down(next.board, gained); break;
        default: return false;
    }
    if (!changed) return false;
    if (game.score > std::numeric_limits<long long>::max() - gained ||
        game.moves == std::numeric_limits<long long>::max())
        throw std::overflow_error("Counter limit reached; move canceled.");
    next.score += gained;
    ++next.moves;
    insert_random(next.board, rng);
    game = next;
    return true;
}
int highest_tile(const Game& game) {
    int highest = 0;
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (game.board[r][c] > highest) highest = game.board[r][c];
    return highest;
}
bool game_over(const Game& game) {
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c) {
            if (game.board[r][c] == 0) return false;
            if (r + 1 < SIZE && game.board[r][c] == game.board[r + 1][c]) return false;
            if (c + 1 < SIZE && game.board[r][c] == game.board[r][c + 1]) return false;
        }
    return true;
}
void print_board(const Game& game) {
    std::cout << "\nScore: " << game.score << " | Successful moves: " << game.moves
              << " | Highest tile: " << highest_tile(game) << '\n';
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            std::cout << '|' << std::setw(11);
            if (game.board[r][c] == 0) std::cout << '.';
            else std::cout << game.board[r][c];
        }
        std::cout << "|\n";
    }
}
bool save_game(const Game& game, const std::string& path, std::string& error) {
    error.clear();
    std::ofstream out(path);
    if (!out) { error = "Cannot open save file for writing."; return false; }
    out << "CS211_2048_V1\n" << game.score << ' ' << game.moves << '\n';
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) out << game.board[r][c] << (c == SIZE - 1 ? '\n' : ' ');
    }
    out.close(); // Includes final flush/close errors in the reported status.
    if (!out) { error = "Failed to finish writing save file."; return false; }
    return true;
}
bool load_game(Game& game, const std::string& path, std::string& error) {
    error.clear();
    std::ifstream in(path);
    if (!in) { error = "Cannot open save file for reading."; return false; }
    Game candidate;
    std::string marker;
    if (!(in >> marker) || marker != "CS211_2048_V1") {
        error = "Invalid save version marker."; return false;
    }
    if (!(in >> candidate.score >> candidate.moves) || candidate.score < 0 || candidate.moves < 0) {
        error = "Invalid, missing, or out-of-range counters."; return false;
    }
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (!(in >> candidate.board[r][c]) || !valid_tile(candidate.board[r][c])) {
                error = "Invalid, missing, or out-of-range board value."; return false;
            }
    in >> std::ws;
    if (in.bad() || !in.eof()) { error = "Extra content or read error in save file."; return false; }
    game = candidate; // Commit only after every check succeeds; no random insertion.
    return true;
}
