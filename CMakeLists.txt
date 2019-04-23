cmake_minimum_required(VERSION 3.13)
project(BuoyancySimulator)

set(CMAKE_CXX_STANDARD 14)

# CGL
add_subdirectory(CGL)
include_directories(CGL/include)
include_directories(CGAL-4.14/include)
include_directories(.)
include_directories(collision)
include_directories(misc)
include_directories(boost_1_70_0)

set(CMAKE_MODULE_PATH "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/cmake/;${CMAKE_MODULE_PATH}")
find_package(Eigen3 3.3.7)
  if (EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
endif()

add_executable(BuoyancySimulator main.cpp)
