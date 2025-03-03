cmake_minimum_required(VERSION 3.16)
project(OrderMatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags (use append to avoid overriding defaults)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Include the 'include/' directory for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all source files
file(GLOB SOURCES src/*.cpp)

# Create the executable
add_executable(order_matching_engine ${SOURCES})
