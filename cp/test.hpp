#define TEST
#include <iostream>
#include <iomanip>
#include <optional>

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>

#include <cmath>
#include <memory>


template <typename T>
bool test_case(
    std::vector<std::pair<T, T>> &positions,
    std::vector<std::pair<size_t, size_t>> &edges,
    std::vector<std::pair<size_t, size_t>> &fromtos,
    std::vector<double> &answers
) {
    size_t nodeCount = positions.size();
    std::vector<Node<T>> nodes;
    nodes.reserve(nodeCount);
    std::vector<std::vector<std::pair<size_t, double>>> graph(nodeCount);

    for (uint32_t i = 0; i < nodeCount; ++i) {
        nodes.emplace_back(i, positions[i].first, positions[i].second);
    }

    for (auto &[from, to] : edges) {
        from--; to--;

        double distance = nodes[from].to(nodes[to]);
        graph[from].emplace_back(to, distance);
        graph[to].emplace_back(from, distance);
    }
    
    AStar astar(graph, nodes);

    for (size_t i = 0; i < fromtos.size(); ++i) {
        std::optional<double> result = astar.shortestPath(fromtos[i].first - 1, fromtos[i].second - 1);
        double answer = answers[i];
        if (!((result.has_value() && std::abs(*result - answer) < 1e-9) || (!result.has_value() && answer == -1.0))) {
            std::cout << i << ": result = " << result.value_or(-1) << " and answer is " << answer << '\n';
            return false;
        }
    }

    return true;
}

typedef double T;

void test() {
    {
        std::vector<std::pair<T, T>> positions = {
            { 0,  0},
            { 1,  1},
            {-1,  1},
            { 0,  2},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 2},
            {1, 3},
            {2, 4},
            {3, 4},
            {1, 4},
        };

        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 4},
        };

        std::vector<double> answers = {
            2,
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }
    
    {
        std::vector<std::pair<T, T>> positions = {
            { 0,  0},
            { 2,  1},
            { 1, -1},
            { 2, -2},
            { 4, -2},
            { 4,  2},
            {-1,  2},
            { 2,  3},
            { 3, -1},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 2},
            {1, 3},
            {1, 4},
            {1, 9},
            {5, 8},
            {9, 6},
            {7, 6},
            {6, 7},
            {6, 9},
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 6},
            {5, 8},
            {1, 5},
            {7, 9},
        };
        std::vector<double> answers = {
            sqrt(10) + sqrt(10),
            sqrt(29),
            -1.0,
            5 + sqrt(10),
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }

    {
        std::vector<std::pair<T, T>> positions = {
            {-1, 0},
            {-1, 1},
            {-1, 2},
            {0, 0},
            {0, 1},
            {0, 2},
            {1, 0},
            {1, 1},
            {1, 2},
            {2, 0},
            {3, 0},
            {3, 1},
            {3, 2},
            {4, 0},
            {4, 1},
            {4, 2},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 4},
            {1, 5},
            {1, 2},
            {2, 4},
            {2, 5},
            {2, 6},
            {2, 3},
            {3, 5},
            {3, 6},
            
            {4, 7},
            {4, 8},
            {4, 5},
            {5, 7},
            {5, 8},
            {5, 9},
            {5, 6},
            {6, 8},
            {6, 9},

            {7, 10},
            {7, 8},
            {8, 10},
            {8, 9},

            {10, 11},
            {10, 12},

            {11, 14},
            {11, 15},
            {11, 12},
            {12, 14},
            {12, 15},
            {12, 16},
            {12, 13},
            {13, 15},
            {13, 16},

            {14, 15},
            {15, 16},
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {4, 16},
            {1, 16},
            {14, 16},
            {8, 13},
            {13, 8},
        };
        std::vector<double> answers = {
            1 + 1 + sqrt(2) + sqrt(2),
            1 + 1 + 1 + sqrt(2) + sqrt(2),
            2,
            sqrt(2) + sqrt(2) + 1,
            sqrt(2) + sqrt(2) + 1,
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }

    {
        std::vector<std::pair<T, T>> positions = {
            { 2,  1},
            { 1, -1},
            { 3, -1},
            { 2, -1},
            { 3,  1},
            { 3,  2},
            { 1,  2},
            { 1,  1},
            {-1,  1},
            {-1, -1},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 2},
            {2, 3},
            {3, 4},
            {4, 5},
            {5, 6},
            {7, 8},
            {9, 7},
            {9, 8},
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 6},
            {1, 7},
            {5, 9},
            {10, 9},
            {10, 3},
            {10, 10},
            {8, 7},
        };
        std::vector<double> answers = {
            sqrt(5) + 2 + 1 + sqrt(5) + 1,
            -1,
            -1,
            -1,
            -1,
            0,
            1,
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }

    {
        std::vector<std::pair<T, T>> positions = {
            { 2,  2},
            { 1, -1},
            { 0,  1},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 2},
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 2},
            {2, 1},
            {3, 3},
            {1, 1},
            {1, 3},
            {2, 2},
        };
        std::vector<double> answers = {
            sqrt(10),
            sqrt(10),
            0,
            0,
            -1,
            0,
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }

    {
        std::vector<std::pair<T, T>> positions = {
            { 2,  2},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 1}
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 1},
        };
        std::vector<double> answers = {
            0,
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }

    {
        std::vector<std::pair<T, T>> positions = {
            {-1, 0},
            {-1, 1},
            {-1, 2},
            {0, 0},
            {0, 1},
            {0, 2},
            {1, 0},
            {1, 1},
            {1, 2},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9},
            {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9},
            {1, 3}, {2, 4}, {3, 5}, {4, 6}, {5, 7}, {6, 8}, {7, 9},
            {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9},
            {1, 5}, {2, 6}, {3, 7}, {4, 8}, {5, 9},
            {1, 6}, {2, 7}, {3, 8}, {4, 9},
            {1, 7}, {2, 8}, {3, 9}, 
            {1, 8}, {2, 9},
            {1, 9},
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 2},
            {2, 3},
            {4, 1},
            {1, 1},
            {5, 9},
            {2, 2},
            {9, 1},
        };
        std::vector<double> answers = {
            1,
            1,
            1,
            0,
            sqrt(2),
            0,
            sqrt(2) + sqrt(2),
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }

    {
        std::vector<std::pair<T, T>> positions = {
            {1, 1},
            {1, 2},
            {2, 2},
            {1, 4},
            {2, 1},
            {3, 0},
            {4, 1},
            {4, 2},
            {3, 3},
            {2, 3},
        };
        std::vector<std::pair<size_t, size_t>> edges = {
            {1, 5},
            {2, 1},
            {3, 5},
            {2, 3},
            {4, 2},
            {7, 8},
            {5, 6},
            {6, 7},
            {10, 9},
            {9, 8},
            {2, 4},
        };
        std::vector<std::pair<size_t, size_t>> fromtos = {
            {1, 10},
            {3, 10},
            {5, 10},
            {8, 3},
        };
        std::vector<double> answers = {
            1 + sqrt(2) + sqrt(2) + 1 + sqrt(2) + 1,
            1 + sqrt(2) + sqrt(2) + 1 + sqrt(2) + 1,
            sqrt(2) + sqrt(2) + 1 + sqrt(2) + 1,
            1 + sqrt(2) + sqrt(2) + 1,
        };

        if (!test_case(positions, edges, fromtos, answers)) {
            std::cout << "epic fail\n";
        } else {
            std::cout << "all gud\n";
        }
    }
}