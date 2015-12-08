cmake_minimum_required (VERSION 2.6)


#output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#option(DEBUG_MODE on)

#setup some debug flags
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11 -fopenmp -Wall")
if(DEBUG_MODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -ffast-math")
endif()

include_directories(Pulsar/include)
add_subdirectory(Pulsar)

add_subdirectory(example)
