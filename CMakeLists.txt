cmake_minimum_required(VERSION 3.28)

project(okapi
    VERSION 1.0.0
    LANGUAGES CXX
)

enable_testing()
include(CTest)

add_subdirectory(src)
add_subdirectory(test)
