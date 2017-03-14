cmake_minimum_required(VERSION 3.5)
project(NetworkSimulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOURCE_FILES Globals.cpp main.cpp EventQueue.cpp Computer.cpp UnionFind.cpp)
add_executable(NetworkSimulator ${SOURCE_FILES})