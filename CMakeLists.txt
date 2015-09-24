cmake_minimum_required(VERSION 3.3)
project(qac)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp lexer.cpp lexer.h parser.cpp parser.h)
add_executable(qac ${SOURCE_FILES})