#include "poolallocator.hpp"
#include <list>
#include <chrono>

using namespace customalloc;

#define VALUES 30
std::chrono::steady_clock::time_point begin, end;

int main() {
    std::cout << "LIST OF " << VALUES << std::endl;
    std::list<size_t> ls1;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i <= VALUES; i++) {
        ls1.push_back(i);
    }
    end = std::chrono::steady_clock::now();

    std::cout << "STL ALLOCATOR" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()
              << "[ns]" << std::endl;

    std::list<size_t, PoolAllocator<size_t>> ls2;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i <= VALUES; i++) {
        ls2.push_back(i);
    }
    end = std::chrono::steady_clock::now();

    std::cout << "CUSTOM ALLOCATOR" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()
              << "[ns]" << std::endl;

    std::cout << '\n';

    std::cout << "VECTOR OF " << VALUES << std::endl;
    std::vector<size_t> vc1;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i <= VALUES; i++) {
        vc1.push_back(i);
    }
    end = std::chrono::steady_clock::now();

    std::cout << "STL ALLOCATOR" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()
              << "[ns]" << std::endl;

    std::vector<size_t, PoolAllocator<size_t>> vc2;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i <= VALUES; i++) {
        vc2.push_back(i);
    }
    end = std::chrono::steady_clock::now();

    std::cout << "CUSTOM ALLOCATOR" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()
              << "[ns]" << std::endl;

    return 0;
}