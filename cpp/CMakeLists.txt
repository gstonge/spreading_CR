cmake_minimum_required(VERSION 3.0)

# Configure the project
#----------------------
project(gillespie-propagation)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
set(LIBRARY_OUTPUT_PATH lib/)
set(EXECUTABLE_OUTPUT_PATH bin/)
find_package(pybind11 REQUIRED)

# Include libraries
#------------------
include_directories(lib/)
link_directories(lib/)

# Generate library
#-----------------------------
file( GLOB_RECURSE LIB_SOURCES lib/*.cpp)
file( GLOB_RECURSE LIB_HEADERS lib/*.hpp)
#add_library( network ${LIB_SOURCES} ${LIB_HEADERS} )
pybind11_add_module(PropagationProcess ${LIB_SOURCES} ${LIB_HEADERS})

# Declare executables
#--------------------
#add_executable( prevalence_SIS src/prevalence_SIS.cpp )

# Link libraries
#---------------
#target_link_libraries( prevalence_SIS network )
