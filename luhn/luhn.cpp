#include "luhn.h"
bool validInput(const std::string& number) {
    if (number.size() != 16) return false;
    for (char digit : number)
        if (digit < '0' || digit > '9') return false;
    return true;
}
int luhnSum(const std::string& number) {
    if (!validInput(number)) return -1;
    int sum = 0;
    bool doubleDigit = false;
    for (int i = 15; i >= 0; --i) {
        int value = number[i] - '0';
        if (doubleDigit) {
            value *= 2;
            if (value > 9) value -= 9;
        }
        sum += value;
        doubleDigit = !doubleDigit;
    }
    return sum;
}
bool checkLuhn(const std::string& number) {
    const int sum = luhnSum(number);
    return sum >= 0 && sum % 10 == 0;
}
