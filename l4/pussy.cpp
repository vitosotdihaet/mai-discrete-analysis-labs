#include <iostream>

#include <vector>
#include <string>


std::vector<size_t> calculateZArray(const std::string& text) {
    size_t textLength = text.length();
    std::vector<size_t> zArray(textLength, 0);

    // z box indecies
    size_t left = 0, right = 0;

    for (size_t index = 1; index < textLength; ++index) {
        // in the z box => copy from it if in bound
        if (index <= right) {
            zArray[index] = std::min(right - index + 1, zArray[index - left]);
        }

        // boundary check and check prefix with current text position
        while (index + zArray[index] < textLength && text[zArray[index]] == text[index + zArray[index]]) {
            ++zArray[index];
        }

        // set up a new z box
        if (index + zArray[index] - 1 > right) {
            left = index;
            right = index + zArray[index] - 1;
        }
    }

    return zArray;
}


std::vector<size_t> findPattern(const std::string& text, const std::string& pattern) {
    std::string concatenated = pattern + '$' + text;

    std::vector<size_t> zArray = calculateZArray(concatenated);

    size_t patternLength = pattern.length();

    std::vector<size_t> positions;

    for (size_t index = patternLength + 1; index < concatenated.length(); ++index) {
        if (zArray[index] == patternLength) { // match
            positions.push_back(index - patternLength - 1);
        }
    }

    return positions;
}


int main() {
    std::string text, pattern;

    std::getline(std::cin, text);
    std::getline(std::cin, pattern);

    std::vector<size_t> positions = findPattern(text, pattern);

    for (size_t position : positions) {
        std::cout << position << '\n';
    }

    return 0;
}
