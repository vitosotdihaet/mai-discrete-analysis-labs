#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <optional>


struct Edge {
    size_t to;
    uint32_t weight;

    Edge(size_t to, uint32_t weight) : to(to), weight(weight) {}
};


std::optional<uint64_t> dijkstra(const size_t vertexCount, const size_t start, const size_t finish, const std::vector<std::vector<Edge>> &graph) {
    // distances to the starting vertex
    std::vector<uint64_t> distances(vertexCount, std::numeric_limits<uint64_t>::max());
    std::priority_queue<std::pair<uint64_t, size_t>, std::vector<std::pair<uint64_t, size_t>>, std::greater<>> distanceToVertex;

    distances[start] = 0;
    distanceToVertex.emplace(0, start);

    while (!distanceToVertex.empty()) {
        const auto [distance, vertex] = distanceToVertex.top();
        if (vertex == finish) { break; }

        distanceToVertex.pop();

        if (distance > distances[vertex]) { continue; }

        for (const Edge &edge : graph[vertex]) {
            size_t adjacentVertex = edge.to;
            uint64_t newDistance = distance + edge.weight;

            if (newDistance < distances[adjacentVertex]) {
                distances[adjacentVertex] = newDistance;
                distanceToVertex.emplace(newDistance, adjacentVertex);
            }
        }
    }

    if (distances[finish] == std::numeric_limits<uint64_t>::max()) {
        return std::nullopt;
    } else {
        return distances[finish];
    }
}

int main() {
    uint32_t vertexCount, edgeCount;
    size_t start, finish;

    std::cin >> vertexCount >> edgeCount >> start >> finish;
    --start; --finish;

    std::vector<std::vector<Edge>> graph(vertexCount);

    for (uint32_t i = 0; i < edgeCount; ++i) {
        size_t from, to;
        uint32_t weight;
        std::cin >> from >> to >> weight;
        --from; --to;
        graph[from].emplace_back(to, weight);
        graph[to].emplace_back(from, weight);
    }

    if (std::optional<uint64_t> answ = dijkstra(vertexCount, start, finish, graph)) {
        std::cout << *answ << '\n';
    } else {
        std::cout << "No solution\n";
    }
}
