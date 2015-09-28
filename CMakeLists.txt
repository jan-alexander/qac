cmake_minimum_required(VERSION 3.3)
project(qac)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES
        include/qac/lexer/lexer.h
        include/qac/parser/parser.h
)

set(SOURCE_FILES
        src/main.cpp
        src/lexer/lexer.cpp
        src/parser/parser.cpp
)

include_directories(include)
add_executable(qac ${SOURCE_FILES} ${HEADER_FILES})