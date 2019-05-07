cmake_minimum_required(VERSION 2.8.4)

project(BuoyancySimulator)

# Find CGAL
find_package(CGAL REQUIRED COMPONENTS Core) # If the dependency is required, use REQUIRED option - if it's not found CMake will issue an error
include( ${CGAL_USE_FILE} )


add_subdirectory(CGL)
include_directories(CGL/include)
include_directories(CGAL-4.14/include)
include_directories(.)
include_directories(collision)
include_directories(misc)

# Setup your executable
include_directories (BEFORE "../../include")
include_directories (BEFORE "include")

include( CGAL_CreateSingleSourceCGALProgram )
create_single_source_cgal_program( "main.cpp" )

find_package(Eigen3 3.3.7)
if (EIGEN3_FOUND)
    include( ${EIGEN3_USE_FILE} )
endif()

set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
