// leetcode 738
#include <cmath>
#include <string>


class Solution {
public:
    void decrease(std::string &numberAsString, const size_t length, const size_t index) {
        numberAsString[index] -= 1;
        for (size_t i = index + 1; i < length; ++i) {
            numberAsString[i] = '9';
        }
    }

    int monotoneIncreasingDigits(int number) {
        std::string numberAsString = std::to_string(number);
        const size_t length = numberAsString.length();

        while (true) {
            size_t i = 0;
            for (; i < length - 1; ++i) {
                if (numberAsString[i] > numberAsString[i + 1]) {
                    decrease(numberAsString, length, i);
                    break;
                }
            }

            if (i == length - 1) {
                break;
            }
        }

        return std::stoi(numberAsString);
    }
};