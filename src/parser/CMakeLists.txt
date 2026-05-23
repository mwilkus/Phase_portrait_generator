cmake_minimum_required(VERSION 3.15)
project(parser)

add_library(parser SHARED parsing.cpp)

target_include_directories(parser PUBLIC include)