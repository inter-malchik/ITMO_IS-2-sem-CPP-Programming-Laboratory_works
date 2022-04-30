cmake_minimum_required(VERSION 3.20)
project(my_project LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(my_project metafuctions.hpp)
target_link_libraries(my_project gtest_main)

enable_testing()

add_executable(tests polynom_test.cc)
target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)