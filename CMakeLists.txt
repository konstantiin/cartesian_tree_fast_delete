cmake_minimum_required(VERSION 3.16)
project(fast-delete-heap)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -DLOCAL)


add_executable(run_test main.cpp)
