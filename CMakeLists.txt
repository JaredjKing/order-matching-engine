cmake_minimum_required(VERSION 3.20)
project(order_matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings + opt (tune as you like)
add_compile_options(-Wall -Wextra -Wpedantic -O3)

add_executable(order_matching_engine
  src/main.cpp
  src/OrderBook.cpp
  src/SkipList.cpp
  src/MarketEvents.cpp
  src/UdpPublisher.cpp
  src/TcpServer.cpp
)

target_include_directories(order_matching_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Linux threads / sockets
target_link_libraries(order_matching_engine PRIVATE pthread)

# Optional: build-time flag to gate the network mode in main.cpp
target_compile_definitions(order_matching_engine PRIVATE ENABLE_NET=1)
