#include <iostream>
#include <iomanip>

#include <vector>
#include <set>
#include <map>
#include <queue>

#include <cmath>
#include <cinttypes>



constexpr size_t SIZE_T_MAX = std::numeric_limits<size_t>::max();
constexpr double DOUBLE_INF = std::numeric_limits<double>::infinity();

template <typename T>
class Node {
private:
    T x = 0, y = 0;
public:
    size_t id = 0;
    double heuristicCost = DOUBLE_INF;
    double pathCost = DOUBLE_INF;

public:
    Node(size_t id, T x, T y) : x(x), y(y), id(id) {}

    double to(Node<T> other) {
        double dx = this->x - other.x;
        double dy = this->y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    friend std::istream& operator>>(std::istream &in, Node<T> &point) {
        in >> point.x >> point.y;
        return in;
    }
};



template <typename T>
class AStar {
private:
    std::vector<std::vector<std::pair<size_t, double>>> &graph;
    std::vector<Node<T>> &nodes;

public:
    AStar(std::vector<std::vector<std::pair<size_t, double>>> &graph, std::vector<Node<T>> &nodes) : graph(graph), nodes(nodes) {}

    double shortestPath(size_t fromID, size_t toID) {
        if (fromID == toID) { return 0; }

        Node<T> &start = this->nodes[fromID];
        Node<T> &end = this->nodes[toID];

        for (Node<T> &n : nodes) {
            n.pathCost = DOUBLE_INF;
            n.heuristicCost = n.to(end);
        }

        start.pathCost = 0;


        std::vector<bool> visited(nodes.size(), false);
        std::vector<size_t> parents(nodes.size(), SIZE_T_MAX);

        auto compareNodesAtIndices = [&](std::pair<size_t, double> p1, std::pair<size_t, double> p2) {
            if (p1.second == p2.second) {
                return p1.first < p2.first;
            }

            return p1.second > p2.second;
        };

        std::priority_queue<std::pair<size_t, double>, std::vector<std::pair<size_t, double>>, decltype(compareNodesAtIndices)> open(compareNodesAtIndices);
        open.emplace(start.id, start.pathCost + start.heuristicCost);

        size_t currentID = 0;
        while (!open.empty()) {
            // get node with lowest path cost + heuristic cost
            currentID = open.top().first;
            if (currentID == toID) { break; }

            open.pop();

            visited[currentID] = true;

            // iterate over all edges
            for (const auto &[nextID, edgeLength] : graph[currentID]) {
                // skip a node at this edge if it was visited before
                if (visited[nextID]) { continue; }

                const double newCost = this->nodes[currentID].pathCost + edgeLength;
                double &nextPathCost = this->nodes[nextID].pathCost;
                if (newCost < nextPathCost) {
                    nextPathCost = newCost;
                    open.emplace(nextID, nextPathCost + this->nodes[nextID].heuristicCost);
                    parents[nextID] = currentID;
                }
            }
        }

        if (currentID != toID) {
            return -1.0f;
        }

        double length = 0;
        size_t parentID = parents[currentID];
        while (parentID != SIZE_T_MAX) {
            length += this->nodes[parentID].to(this->nodes[currentID]);
            currentID = parentID;
            parentID = parents[currentID];
        }

        return length;
    }
};


// #include "test.hpp"

int main() {
#ifdef TEST
    test();
    return 0;
#endif
    uint32_t nodeCount, edgeCount;
    std::cin >> nodeCount >> edgeCount;

    std::vector<Node<double>> nodes;
    nodes.reserve(nodeCount);
    std::vector<std::vector<std::pair<size_t, double>>> graph(nodeCount);

    for (uint32_t i = 0; i < nodeCount; ++i) {
        double x, y;
        std::cin >> x >> y;
        nodes.emplace_back(i, x, y);
    }

    for (uint32_t i = 0; i < edgeCount; ++i) {
        size_t from, to;
        std::cin >> from >> to;
        from--; to--;

        double distance = nodes[from].to(nodes[to]);
        graph[from].emplace_back(to, distance);
        graph[to].emplace_back(from, distance);
    }


    AStar astar(graph, nodes);

    uint32_t requestCount;
    std::cin >> requestCount;

    for (uint32_t i = 0; i < requestCount; ++i) {
        uint32_t from, to;
        std::cin >> from >> to;
        from--; to--;

        std::cout << std::fixed << std::setprecision(6) << astar.shortestPath(from, to) << '\n';
    }
}