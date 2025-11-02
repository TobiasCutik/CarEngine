cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

add_subdirectory(CarEngine)
add_subdirectory(SampleProject)