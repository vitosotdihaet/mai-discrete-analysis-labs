// leetcode 467
#include <string>


class Solution {
public:
    int findSubstringInWraproundString(std::string s) {
        size_t currentCount = 1;

        size_t dp[26]; // each element is a number of strings that are a substring of base ending at index
        dp[s[0] - 'a'] = currentCount; // a single letter

        for (size_t i = 1; i < s.size(); ++i) {
            const char currentLetterAsIndex = s[i] - 'a';
            const char previousLetterAsIndex = s[i - 1] - 'a';

            if ((previousLetterAsIndex + 1) % 26 == currentLetterAsIndex) { // previous and current are ascending
                currentCount++;
            } else { // previous and current are not ascending => reset the counter
                currentCount = 1;
            }

            if (dp[currentLetterAsIndex] < currentCount) {
                dp[currentLetterAsIndex] = currentCount;
            }
        }

        size_t count = 0;
        for (const size_t e : dp) {
            count += e;
        }

        return count;
    }
};