cmake_minimum_required(VERSION 3.15)
project(cordinats)

add_library(cordinats SHARED
    cordinats.cpp
)

target_include_directories(cordinats INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)