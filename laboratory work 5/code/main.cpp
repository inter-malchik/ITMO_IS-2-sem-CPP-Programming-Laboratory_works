#include "poolallocator.hpp"
#include <list>
#include <chrono>

using namespace customalloc;

int main() {
    /*
    PoolAllocator<int> assalloc(10, 2);
    int *vct = assalloc.allocate(5);
    int *ass2 = assalloc.allocate(3);
    int *ass3 = assalloc.allocate(1);
    int *ass4 = assalloc.allocate(1);
    vct[0] = 5;
    ass2[0] = 6;
    ass3[0] = 7;
    ass4[0] = 8;

    assalloc.deallocate(ass4, 1);
    assalloc.deallocate(ass3, 1);
    */

    /*
    std::vector<int, PoolAllocator<int>> vct = {1, 2, 3, 4, 5};
    for (int as : vct)
        std::cout << as << '\n';


    std::list<int, PoolAllocator<int>> ls;
    ls.push_back(1);
    ls.push_back(2);
    ls.push_back(2);
    for (int as : ls)
        std::cout << as << '\n';
    */
    std::chrono::steady_clock::time_point begin, end;

    std::list<size_t> ls1;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i <= 10; i++) {
        ls1.push_back(i);
    }
    end = std::chrono::steady_clock::now();

    std::cout << "STL ALLOCATOR" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;



    std::list<size_t, PoolAllocator<size_t>> ls2;

    begin = std::chrono::steady_clock::now();
    for (size_t i = 1; i <= 10; i++) {
        ls2.push_back(i);
    }
    end = std::chrono::steady_clock::now();

    std::cout << "CUSTOM ALLOCATOR" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;

    return 0;
}