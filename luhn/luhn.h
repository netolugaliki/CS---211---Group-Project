#ifndef CS211_LUHN_H
#define CS211_LUHN_H
#include <string>
bool validInput(const std::string& number);
// Return -1 for malformed input; otherwise return the transformed digit sum.
int luhnSum(const std::string& number);
bool checkLuhn(const std::string& number);
#endif
