cmake_minimum_required(VERSION 3.15)
project(control)

find_package(SDL2 REQUIRED)

add_library(control SHARED
    control.cpp
)

target_link_libraries(control PRIVATE SDL2::SDL2)

target_include_directories(control INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)