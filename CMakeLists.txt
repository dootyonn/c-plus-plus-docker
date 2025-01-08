cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)

project(
  okapi
  VERSION 1.0.0
  LANGUAGES CXX
)

add_subdirectory(src)
add_subdirectory(test)
