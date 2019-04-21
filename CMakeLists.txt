cmake_minimum_required(VERSION 3.13)
project(BuoyancySimulator)

set(CMAKE_CXX_STANDARD 14)

# CGL
add_subdirectory(CGL)
include_directories(CGL/include)
include_directories(.)
include_directories(collision)
include_directories(misc)

add_executable(BuoyancySimulator main.cpp)
