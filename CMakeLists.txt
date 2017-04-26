cmake_minimum_required(VERSION 3.2)
project(Polygon)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp util.cpp util.h Polygon.cpp)
add_executable(Polygon ${SOURCE_FILES})
