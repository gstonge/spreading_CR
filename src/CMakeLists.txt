cmake_minimum_required(VERSION 3.1)

# Configure the project
#----------------------
project(main_test)

include_directories(.)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test main_test.cpp BinaryTree.cpp evolution.cpp HashPropensity.cpp io_data.cpp Network.cpp SpreadingProcess.cpp StaticNetworkSIR.cpp)
