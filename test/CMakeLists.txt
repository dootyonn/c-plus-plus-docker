cmake_minimum_required(VERSION 3.28)

find_package(GTest REQUIRED)
cmake_policy(SET CMP0167 NEW)
find_package(Boost
REQUIRED 
    system
    thread
    headers
    coroutine
)
find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/example.proto)

add_executable(okapitest
    trait_test.cpp
    functional_raii.cpp
    range_test.cpp
    variadic_test.cpp
    sync_test.cpp
    lambda_test.cpp
    # logger_test.cpp
    cons_dest.cpp
    vec.cpp
    # allocators_test.cpp
    #awaitable_test.cpp
    proto_test.cpp
    ${PROTO_SRCS}
)

target_compile_features(okapitest
PUBLIC
    cxx_std_23
)

target_include_directories(okapitest
PRIVATE
    ../src
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(okapitest
PRIVATE
    protobuf::libprotobuf
    Boost::system
    Boost::thread
    Boost::headers
    gtest
    gtest_main
)

gtest_discover_tests(okapitest)