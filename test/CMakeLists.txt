cmake_minimum_required(VERSION 3.28)

find_package(GTest REQUIRED)

add_executable(
  okapitest
    trait_test.cpp
    functional_raii.cpp
    range_test.cpp
    variadic_test.cpp
    sync_test.cpp
    lambda_test.cpp
)

target_include_directories(
  okapitest
  PRIVATE
    ../src
)

target_link_libraries(okapitest gtest gtest_main)

gtest_discover_tests(okapitest)