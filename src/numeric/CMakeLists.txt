cmake_minimum_required(VERSION 3.15)
project(numeric)

add_library(numeric SHARED
    numeric.cpp
)

target_link_libraries(numeric PUBLIC cordinats control)

target_include_directories(numeric INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)