cmake_minimum_required(VERSION 3.20)
project("currencies" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

# find python libraries
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(PythonLibs 3.0 REQUIRED)
include_directories(${PYTHON3_INCLUDE_DIRS} ${NumPy_INCLUDE_DIRS})

find_package(CURL REQUIRED)

# populate repository
include(FetchContent)
FetchContent_Declare(
        matplotlib
        GIT_REPOSITORY https://github.com/lava/matplotlib-cpp.git
        GIT_TAG        f23347fca25219d1c42cbb91608b5556814bf572
)
FetchContent_GetProperties(matplotlib)
if(NOT matplotlib_POPULATED)
    FetchContent_Populate(matplotlib)
endif()
include_directories(SYSTEM ${matplotlib_SOURCE_DIR})

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(${CURL_INCLUDE_DIR})

# add executable
add_executable(currencies "main.cpp" "curstats.hpp" "curstats.cpp" "easyrequest.hpp" "easyrequest.cpp")

# link
target_link_libraries(currencies PRIVATE nlohmann_json::nlohmann_json ${CURL_LIBRARIES} ${PYTHON_LIBRARIES} Python3::NumPy)
