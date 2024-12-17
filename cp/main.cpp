#include <iostream>
#include <iomanip>
#include <optional>

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <cmath>
#include <memory>



constexpr size_t SIZE_T_MAX = std::numeric_limits<size_t>::max();
constexpr double DOUBLE_INF = std::numeric_limits<double>::infinity();

template <typename T>
class Node {
private:
    T x = 0, y = 0;
public:
    size_t id = 0;
    static size_t globalID;
    double heuristicCost = DOUBLE_INF;
    double pathCost = DOUBLE_INF;

private:
public:
    Node() : id(Node<T>::globalID++) {}

    Node(T x, T y) : x(x), y(y), id(Node<T>::globalID++) {}

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
size_t Node<T>::globalID = 0;


template <typename T>
class AStar {
private:
    std::vector<std::vector<std::pair<size_t, double>>> &graph;
    std::vector<Node<T>> &nodes;

public:
    AStar(std::vector<std::vector<std::pair<size_t, double>>> &graph, std::vector<Node<T>> &nodes) : graph(graph), nodes(nodes) {}

    std::optional<double> shortestPath(size_t fromIndex, size_t toIndex) {
        if (fromIndex == toIndex) { return 0; }

        Node<T> &start = this->nodes[fromIndex];
        Node<T> &end = this->nodes[toIndex];

        for (Node<T> &n : nodes) {
            n.pathCost = DOUBLE_INF;
            n.heuristicCost = n.to(end);
        }

        start.pathCost = 0;

        std::vector<bool> visited(nodes.size(), false);
        std::vector<size_t> parents(nodes.size(), SIZE_T_MAX);


        auto compareNodesAtIndices = [&](size_t firstIndex, size_t secondIndex) {
            if (firstIndex == secondIndex) { return false; }

            const Node<T> &first = this->nodes[firstIndex];
            const Node<T> &second = this->nodes[secondIndex];

            const double fCost = first.pathCost + first.heuristicCost;
            const double sCost = second.pathCost + second.heuristicCost;

            if (fCost == sCost) {
                return firstIndex < secondIndex;
            }

            return fCost < sCost;
        };

        std::set<size_t, decltype(compareNodesAtIndices)> open(compareNodesAtIndices);
        open.insert(start.id);

        size_t currentID = 0;
        while (!open.empty()) {
            // get node with lowest path cost + heuristic cost
            currentID = *open.begin();
            open.erase(open.begin());

            if (currentID == toIndex) { break; }

            visited[currentID] = true;

            const Node<T> &current = this->nodes[currentID];

            // iterate over all edges
            for (const auto &[nextID, edgeLength] : graph[currentID]) {
                // skip a node at this edge if it was visited before
                if (visited[nextID]) { continue; }

                const double newCost = current.pathCost + edgeLength;

                Node<T> &next = this->nodes[nextID];
                if (newCost < next.pathCost) {
                    parents[nextID] = currentID;
                    open.erase(nextID);
                    next.pathCost = current.pathCost + edgeLength;
                    open.insert(nextID);
                }
            }
        }

        if (currentID != toIndex) {
            return std::nullopt;
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
        nodes.emplace_back(x, y);
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

        std::optional<double> result = astar.shortestPath(from, to);
        if (result.has_value()) {
            std::cout << std::fixed << std::setprecision(6) << *result << '\n';
        } else {
            std::cout << -1.0 << '\n';
        }
    }
}