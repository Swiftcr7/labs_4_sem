cmake_minimum_required(VERSION 3.25)
project(allocator_border_descriptor)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(allocator_border_descriptor
        allocator_border_descriptor.h
        main.cpp)
