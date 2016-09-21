cmake_minimum_required(VERSION 3.6)
project(CS530prog1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp file_parser.cc file_parser.h file_parse_exception.h)
add_executable(CS530prog1 ${SOURCE_FILES})