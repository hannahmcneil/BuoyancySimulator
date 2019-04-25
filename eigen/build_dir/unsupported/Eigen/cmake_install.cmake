# Install script for directory: /Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/AdolcForward"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/AlignedVector3"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/ArpackSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/AutoDiff"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/BVH"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/EulerAngles"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/FFT"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/IterativeSolvers"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/KroneckerProduct"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/LevenbergMarquardt"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/MatrixFunctions"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/MoreVectorization"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/MPRealSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/NonLinearOptimization"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/NumericalDiff"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/OpenGLSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/Polynomials"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/Skyline"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/SparseExtra"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/SpecialFunctions"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/build_dir/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

