# Install script for directory: /Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Cholesky"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/CholmodSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Core"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Dense"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Eigen"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Eigenvalues"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Geometry"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Householder"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/IterativeLinearSolvers"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Jacobi"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/LU"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/MetisSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/OrderingMethods"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/PaStiXSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/PardisoSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/QR"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/QtAlignedMalloc"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SPQRSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SVD"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/Sparse"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SparseCholesky"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SparseCore"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SparseLU"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SparseQR"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/StdDeque"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/StdList"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/StdVector"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/SuperLUSupport"
    "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "/Users/HannahMcNeil/Desktop/CS184/BuoyancySimulator/eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

