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



template <typename T>
class Node {
private:
    T x = 0, y = 0;
public:
    size_t id = 0;
    static size_t globalID;
    double heuristicCost = std::numeric_limits<double>::infinity();
    double pathCost = std::numeric_limits<double>::infinity();

    std::set<std::pair<size_t, double>> edges;

private:
public:
    Node() : id(Node<T>::globalID++) {}

    Node(T x, T y) : x(x), y(y), id(Node<T>::globalID++) {}

    double to(Node<T> other) {
        return sqrt((this->x - other.x) * (this->x - other.x) + (this->y - other.y) * (this->y - other.y));
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
    std::vector<Node<T>> nodes;

    struct Compare {
        const std::vector<Node<T>>& nodes;

        Compare(const std::vector<Node<T>>& nodes) : nodes(nodes) {}

        bool operator()(size_t firstIndex, size_t secondIndex) const {
            const auto& first = this->nodes[firstIndex];
            const auto& second = this->nodes[secondIndex];
            // std::cout << "compare " << first.id << ' ' << second.id << '\n';
            // std::cout << (first.pathCost + first.heuristicCost) << " vs " << (second.pathCost + second.heuristicCost) << ' ' << ((first.pathCost + first.heuristicCost) > (second.pathCost + second.heuristicCost)) << '\n';
            return (first.pathCost + first.heuristicCost) > (second.pathCost + second.heuristicCost);
        }
    };

    Compare compare;

public:
    AStar(std::vector<Node<T>> &nodes) : nodes(nodes), compare(nodes) {}

    std::optional<double> shortestPath(size_t fromIndex, size_t toIndex) {
        if (fromIndex == toIndex) { return 0; }

        Node<T> &start = this->nodes[fromIndex];
        Node<T> &end = this->nodes[toIndex]; 

        for (Node<T> &n : nodes) {
            n.pathCost = std::numeric_limits<double>::infinity();
            n.heuristicCost = n.to(end);
        }

        start.pathCost = 0;
        start.heuristicCost = start.to(end);

        std::multiset<size_t, Compare> open(compare);
        open.insert(start.id);

        size_t currentID = 0;

        // all node ids that were visited
        std::set<size_t> visited;

        // child id -> parent id
        std::map<size_t, size_t> parents;

        while (!open.empty()) {
            // get node with lowest path cost + heuristic cost
            currentID = *open.begin();
            open.erase(open.begin());

            if (currentID == toIndex) {
                break;
            }

            visited.insert(currentID);

            const Node<T> &current = this->nodes[currentID];

            // iterate over all edges
            for (const auto &[nextID, edgeLength] : current.edges) {
                // skip a node at this edge if it was visited before
                if (visited.count(nextID)) { continue; }

                Node<T> &next = this->nodes[nextID];

                double newCost = current.pathCost + edgeLength;

                if (newCost < next.pathCost) {
                    next.pathCost = current.pathCost + edgeLength;
                    parents[nextID] = currentID;

                    for (auto it = open.begin(); it != open.end(); ++it) {
                        if (*it == nextID) {
                            open.erase(it);
                            break;
                        }
                    }

                    open.insert(nextID);
                }
            }
        }

        if (currentID != toIndex) {
            return std::nullopt;
        }

        double length = 0;
        auto it = parents.find(currentID);
        while (it != parents.end()) {
            size_t parentID = it->second;
            Node<T> &parent = this->nodes[parentID];

            length += parent.to(this->nodes[currentID]);
            // std::cout << parent.id << '\n';

            currentID = parent.id;
            it = parents.find(currentID);
        }

        return length;
    }
};


#include "test.hpp"

int main() {
#ifdef TEST
    test();
    return 0;
#endif
    uint32_t nodeCount, edgeCount;
    std::cin >> nodeCount >> edgeCount;

    std::vector<Node<double>> nodes;
    nodes.reserve(nodeCount);

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
        nodes[from].edges.emplace(to, distance);
        nodes[to].edges.emplace(from, distance);
    }


    AStar astar(nodes);

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