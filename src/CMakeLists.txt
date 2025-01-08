cmake_minimum_required(VERSION 3.28)

# find_package(
#   Boost 1.83.0 REQUIRED COMPONENTS
#   system
#   thread
#   chrono
#   random
# )

find_package(OpenSSL REQUIRED)

find_package(cpprestsdk REQUIRED)

add_executable(okapi ok_api.cpp)

target_include_directories(
okapi
PRIVATE
  .
)

target_link_libraries(
okapi
PRIVATE
  # Boost::system 
  # Boost::thread
  # Boost::chrono
  # Boost::random
  OpenSSL::SSL
  cpprest
)

