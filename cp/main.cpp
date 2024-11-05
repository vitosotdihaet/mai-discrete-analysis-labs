#include <iostream>
#include <vector>



template <typename T>
class Edge {
private:
    T from = 0, to = 0;

public:
    Edge() {}

    Edge(T x, T y) : from(x), to(y) {}

    friend std::istream& operator>>(std::istream &in, Edge<T> &edge) {
        in >> edge.from >> edge.to;
        return in;
    }
};


template <typename T>
class Point {
private:
    T x = 0, y = 0;

public:
    Point() {}

    Point(T x, T y) : x(x), y(y) {}

    friend std::istream& operator>>(std::istream &in, Point<T> &point) {
        in >> point.x >> point.y;
        return in;
    }
};



int main() {
    uint16_t vertexCount, edgeCount;
    std::cin >> vertexCount >> edgeCount;

    std::vector<Point<int32_t>> verticies;
    verticies.reserve(vertexCount);

    for (uint16_t i = 0; i < vertexCount; ++i) {
        int32_t x, y;
        std::cin >> x >> y;
        verticies.emplace_back(x, y);
    }

    std::vector<Edge<uint16_t>> edges;
    edges.reserve(edgeCount);

    for (uint16_t i = 0; i < edgeCount; ++i) {
        uint16_t from, to;
        std::cin >> from >> to;
        edges.emplace_back(from, to);
    }


    uint16_t requestCount;
    std::cin >> requestCount;

    for (uint16_t i = 0; i < requestCount; ++i) {
        Edge<int16_t> request;
        std::cin >> request;

        //* process the request...
    }
}