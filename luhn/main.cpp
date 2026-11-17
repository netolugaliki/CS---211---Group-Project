#include "luhn.h"
#include <iostream>
#include <string>
int main() {
    std::cout << "Luhn checksum validator (16 digits). Enter e to exit.\n"
              << "A passing checksum does not establish that a card exists or is authorized.\n";
    std::string number;
    while (std::cout << "Number: " && std::getline(std::cin, number)) {
        if (number == "e") break;
        if (!validInput(number)) {
            std::cout << "Invalid format: enter exactly 16 digits (0-9), without spaces.\n";
            continue;
        }
        std::cout << "Checksum " << (checkLuhn(number) ? "PASSES" : "FAILS")
                  << " (sum = " << luhnSum(number) << ").\n";
    }
    std::cout << "Goodbye.\n";
}
