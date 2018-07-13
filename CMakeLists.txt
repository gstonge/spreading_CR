cmake_minimum_required(VERSION 3.1)

# Configure the project
#----------------------
project(gillespie_propagation)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )

# Include libraries and source
#-----------------------------
add_subdirectory(pybind11)
include_directories(src/)
link_directories(src/)
file( GLOB_RECURSE LIB_SOURCES src/*.cpp)
file( GLOB_RECURSE LIB_HEADERS src/*.hpp)

# Generate library
#-----------------------------
pybind11_add_module(gillespie_propagation ${LIB_SOURCES} ${LIB_HEADERS})
