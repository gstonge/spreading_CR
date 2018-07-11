cmake_minimum_required(VERSION 3.1)

# Configure the project
#----------------------
project(propagation_simulation)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
set(LIBRARY_OUTPUT_PATH lib/)
set(EXECUTABLE_OUTPUT_PATH bin/)

# Generate libraries
#-----------------------------
file( GLOB_RECURSE LIB_SOURCES lib/*.cpp)
file( GLOB_RECURSE LIB_HEADERS lib/*.hpp)
add_library( network ${LIB_SOURCES} ${LIB_HEADERS} )

# Generate file list for main files
#----------------------------------
#file( GLOB source_files src/* )

# Include libraries
#------------------
include_directories(lib/)
link_directories(lib/)

# Declare executables
#--------------------
add_executable( test_binary_tree src/test_binary_tree.cpp )
add_executable( prevalence_SIS src/prevalence_SIS.cpp )
add_executable( final_size_SIR src/final_size_SIR.cpp )
add_executable(	time_SIS src/time_SIS.cpp )
add_executable(	time_SIR src/time_SIR.cpp )
add_executable( critical_time_SIS src/critical_time_SIS.cpp)
add_executable( critical_time_SIR src/critical_time_SIR.cpp)
add_executable( rejection_probability src/rejection_probability.cpp)



# Link libraries
#---------------
target_link_libraries( test_binary_tree network )
target_link_libraries( prevalence_SIS network )
target_link_libraries( final_size_SIR network )
target_link_libraries( time_SIS network)
target_link_libraries( time_SIR network)
target_link_libraries( critical_time_SIS network)
target_link_libraries( critical_time_SIR network)
target_link_libraries( rejection_probability network)


