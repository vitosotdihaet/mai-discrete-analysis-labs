#include <iostream>
#include <vector>
#include <string>



uint64_t square(std::pair<uint64_t, uint64_t> p) {
    return p.first * p.second;
}

uint64_t maxZeroRectangleSquare(const std::vector<std::vector<bool>> &grid) {
    uint16_t width = grid[0].size(), height = grid.size();
    std::vector<std::vector<std::pair<uint64_t, uint64_t>>> squares(height, std::vector<std::pair<uint64_t, uint64_t>>(width, { 0, 0 }));
    uint64_t maxSquare = 0;

    if (grid[0][0] == false) squares[0][0] = { 1, 1 };

    // fill first column
    for (size_t i = 1; i < height; ++i) {
        if (grid[i][0] == false) {
            if (grid[i - 1][0] == false) {
                squares[i][0] = squares[i - 1][0];
                squares[i][0].first++;
            } else {
                squares[i][0] = { 1, 1 };
            }
        }   
    }

    // fill first row
    for (size_t j = 1; j < width; ++j) {
        if (grid[0][j] == false) {
            if (grid[0][j - 1] == false) {
                squares[0][j] = squares[0][j - 1];
                squares[0][j].second++;
            } else {
                squares[0][j] = { 1, 1 };
            }
        }   
    }

    for (size_t i = 1; i < height; ++i) {
        for (size_t j = 1; j < width; ++j) {
            if (grid[i][j] == true) continue;

            if (grid[i - 1][j - 1] == false && grid[i - 1][j] == false && grid[i][j - 1] == false) {
                squares[i][j].first = std::min(squares[i - 1][j].first, squares[i][j - 1].first) + 1;
                squares[i][j].second = std::min(squares[i - 1][j].second, squares[i][j - 1].second) + 1;
            } else if (grid[i - 1][j - 1] == true && grid[i - 1][j] == false && grid[i][j - 1] == false) {
                if (
                    (squares[i - 1][j].first + 1) * squares[i - 1][j].second - square(squares[i - 1][j]) >
                    squares[i][j - 1].first * (squares[i][j - 1].second + 1) - square(squares[i][j - 1])
                ) {
                    squares[i][j].first = squares[i - 1][j].first + 1;
                    squares[i][j].second = squares[i - 1][j].second;
                } else {
                    squares[i][j].first = squares[i][j - 1].first;
                    squares[i][j].second = squares[i][j - 1].second + 1;
                }
            } else if (grid[i - 1][j] == false && grid[i][j - 1] == true) {
                squares[i][j].first = squares[i - 1][j].first + 1;
                squares[i][j].second = 1;
            } else if (grid[i - 1][j] == true && grid[i][j - 1] == false) {
                squares[i][j].first = 1;
                squares[i][j].second = squares[i][j - 1].second + 1;
            } else {
                squares[i][j] = { 1, 1 };
            }

            maxSquare = std::max(maxSquare, square(squares[i][j]));
        }
    }


    return maxSquare;
}   



int main() {
    uint16_t width, height;
    std::cin >> height >> width;

    std::vector<std::vector<bool>> grid(height, std::vector<bool>(width));

    for (uint16_t i = 0; i < height; ++i) {
        std::string line;
        std::cin >> line;
        for (uint16_t j = 0; j < width; ++j) {
            grid[i][j] = line[j] - '0';
        }
    }

    std::cout << maxZeroRectangleSquare(grid) << '\n';

    return 0;
}