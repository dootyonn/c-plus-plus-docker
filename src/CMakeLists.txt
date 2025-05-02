cmake_minimum_required(VERSION 3.28)

include(CMakePrintHelpers)

# find_package(
#   Boost 1.83.0 REQUIRED COMPONENTS
#   system
#   thread
#   chrono
#   random
# )

find_package(OpenSSL REQUIRED)
cmake_policy(SET CMP0167 NEW)
find_package(cpprestsdk REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl REQUIRED)

add_executable(okapi
    ok_api.cpp
)

target_compile_features(okapi
PRIVATE
    cxx_std_23
)

target_include_directories(okapi
PRIVATE
    .
)

target_link_libraries(okapi
PRIVATE
    # Boost::system 
    # Boost::thread
    # Boost::headers
    # Boost::chrono
    # Boost::random
    OpenSSL::SSL
    cpprest
)

install(
    TARGETS okapi
    RUNTIME DESTINATION bin COMPONENT Runtime
) 

add_executable(testserver 
  server.cpp
)

target_compile_features(testserver
PRIVATE
    cxx_std_23
)

########### routeguidemsg ###########

add_library(routeguidemsg
    helper.cpp
    proto/route_guide.proto
)

target_compile_features(routeguidemsg
PRIVATE
    cxx_std_23
)

target_include_directories(routeguidemsg
PUBLIC
    .
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(routeguidemsg
PUBLIC
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
cmake_print_variables(grpc_cpp_plugin_location)

protobuf_generate(TARGET routeguidemsg LANGUAGE cpp)
protobuf_generate(TARGET routeguidemsg LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

########### routeguideserver ###########

add_executable(routeguideserver
    
    route_guide_server.cpp
)

target_compile_features(routeguideserver
PRIVATE
    cxx_std_23
)

target_include_directories(routeguideserver
PRIVATE
    .
)

target_link_libraries(routeguideserver
PRIVATE
    absl::flags_parse
    absl::absl_log
    absl::log_initialize
    absl::log_globals
    routeguidemsg
)

########### routeguideclient ###########

add_executable(routeguideclient
    helper.cpp
    route_guide_client.cpp
)

target_compile_features(routeguideclient
PRIVATE
    cxx_std_23
)

target_include_directories(routeguideclient
PRIVATE
    .
)

target_link_libraries(routeguideclient
PRIVATE
    absl::flags_parse
    absl::absl_log
    absl::log_initialize
    absl::log_globals
    routeguidemsg
)