#include <iostream>
#include <vector>
#include <string>
#include <stack>



uint32_t maxRectangleInHistogram(const std::vector<uint16_t> &histogram) {
    std::stack<uint16_t> biggestHeightIndecies;

    uint32_t maxSquare = 0;
    const uint16_t width = histogram.size();

    for (uint16_t currentHeightIndex = 0; currentHeightIndex <= width; ++currentHeightIndex) {
        uint16_t currentHeight;
        if (currentHeightIndex != width) {
            currentHeight = histogram[currentHeightIndex];
        } else { // last bar height is always zero
            currentHeight = 0;
        }

        // current height is smaller, so calculate the max square and remove all heights that are bigger than the current height
        while (!biggestHeightIndecies.empty() && histogram[biggestHeightIndecies.top()] >= currentHeight) {
            const uint16_t biggestHeight = histogram[biggestHeightIndecies.top()];
            biggestHeightIndecies.pop();

            uint16_t currentWidth;
            if (biggestHeightIndecies.empty()) { // this index is the biggest width so far
                currentWidth = currentHeightIndex;
            } else { // take the last height index that was smaller than the biggest height
                currentWidth = currentHeightIndex - biggestHeightIndecies.top() - 1;
            }

            maxSquare = std::max(maxSquare, (uint32_t) biggestHeight * (uint32_t) currentWidth);
        }

        biggestHeightIndecies.push(currentHeightIndex);
    }

    return maxSquare;
}


uint32_t maxZeroRectangleSquare(const std::vector<std::vector<bool>> &grid) {
    uint16_t width = grid[0].size(), height = grid.size();
    // histogram is a row of heights of current rectangle
    std::vector<uint16_t> histogram(width, 0);
    uint32_t maxSquare = 0;

    for (uint16_t i = 0; i < height; ++i) {
        for (uint16_t j = 0; j < width; ++j) {
            if (grid[i][j] == false) { // accumulate the height
                histogram[j] += 1;
            } else { // reset the height
                histogram[j] = 0;
            }
        }
        maxSquare = std::max(maxSquare, maxRectangleInHistogram(histogram));
    }

    return maxSquare;
}   



int main() {
    uint16_t width, height;
    std::cin >> height >> width;

    std::vector<std::vector<bool>> grid(height, std::vector<bool>(width));

    for (uint16_t i = 0; i < height; ++i) {
        for (uint16_t j = 0; j < width; ++j) {
            char currentElement;
            std::cin >> currentElement;
            grid[i][j] = currentElement - '0';
        }
    }

    std::cout << maxZeroRectangleSquare(grid) << '\n';

    return 0;
}
