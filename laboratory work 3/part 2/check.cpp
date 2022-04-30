#include <vector>
#include <iostream>

#include "useralgo.hpp"

struct CPoint {
    int x;
    int y;

    bool operator>(const CPoint &other) const {
        return x * x + y * y > other.x * other.x + other.y * other.y;
    }

    bool operator<(const CPoint &other) const {
        return x * x + y * y < other.x * other.x + other.y * other.y;
    }
};

template<typename T>
struct even {
    bool operator()(T a) {
        return a % 2 == 0;
    }
};

template<typename T>
bool evenn(T a) {
    return a % 2 == 0;
}

template<typename T>
struct more {
    bool operator()(const T &a, const T &b) {
        return a > b;
    }
};

int main() {
    std::vector<CPoint> points = {{1, 2},
                                  {1, 3},
                                  {3, 3},
                                  {5, 5}};
    std::cout << ualgo::is_sorted(points.begin(), points.end()) << ' ';

    std::vector<float> lol = {1.0, 2.0, 3.0};
    std::cout << ualgo::is_sorted(lol.begin(), lol.end(), std::less<>()) << ' ';

    std::vector<unsigned> hi = {5, 4, 3, 2, 1};
    std::cout << ualgo::is_sorted(hi.begin(), hi.end(), more<unsigned>()) << std::endl;

    std::vector<int> p = {2, 4, 6, 9, 7, 3};
    std::cout << ualgo::is_partitioned(p.begin(), p.end(), evenn<int>);
}