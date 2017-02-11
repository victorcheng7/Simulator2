cmake_minimum_required(VERSION 3.6)
project(NetworkSimulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp EventQueue.cpp Computer.cpp Globals.cpp)
add_executable(NetworkSimulator ${SOURCE_FILES})