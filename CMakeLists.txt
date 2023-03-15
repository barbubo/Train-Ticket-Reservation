cmake_minimum_required(VERSION 3.23)
project(L7)

set(CMAKE_CXX_STANDARD 14)

add_executable(L7 main.cpp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")