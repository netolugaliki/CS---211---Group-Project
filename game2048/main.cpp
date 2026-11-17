#include "2048_helper.h"
#include <iostream>
#include <stdexcept>
#include <string>

void help() {
    std::cout << "\nu/d/l/r: move up/down/left/right\ns: save | o: open/load | n: new game\n"
              << "h: help | e: exit\nEnter one lowercase command per line.\n"
              << "Save uses savegame.txt in the current working directory.\n"
              << "Saving again replaces this slot. Failed loads preserve your game.\n";
}
void show(const Game& game, bool& announced) {
    print_board(game);
    if (highest_tile(game) >= 2048 && !announced) {
        std::cout << "Success! You reached 2048 or higher. You may keep playing.\n";
        announced = true;
    }
    if (game_over(game)) std::cout << "Game over. Save, load, new game, help, or exit are available.\n";
}
int main() {
    std::mt19937 rng(std::random_device{}());
    Game game;
    new_game(game, rng);
    bool announced = false;
    help();
    show(game, announced);
    std::string command;
    while (std::cout << "Command: " && std::getline(std::cin, command)) {
        if (command == "e") break;
        if (command == "h") { help(); continue; }
        if (command == "n") {
            std::cout << "Start a new game? Type y to confirm: ";
            std::string answer;
            if (!std::getline(std::cin, answer)) break;
            if (answer == "y") {
                new_game(game, rng);
                announced = false;
                show(game, announced);
            } else std::cout << "New game canceled.\n";
        } else if (command == "s" || command == "o") {
            std::string error;
            if (command == "s") {
                if (save_game(game, "savegame.txt", error)) std::cout << "Game saved.\n";
                else std::cout << error << '\n';
            } else if (load_game(game, "savegame.txt", error)) {
                std::cout << "Game loaded.\n";
                announced = false;
                show(game, announced);
            } else std::cout << error << " Game unchanged.\n";
        } else if (command == "u" || command == "d" || command == "l" || command == "r") {
            if (game_over(game)) std::cout << "Game over: movement is disabled.\n";
            else {
                try {
                    if (!play_move(game, command[0], rng)) std::cout << "No change; no tile inserted.\n";
                } catch (const std::overflow_error& error) { std::cout << error.what() << '\n'; }
            }
            show(game, announced);
        } else std::cout << "Invalid command. Type h for help.\n";
    }
    std::cout << "Goodbye.\n";
}
