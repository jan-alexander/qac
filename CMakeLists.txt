cmake_minimum_required(VERSION 3.3)
project(qac)

find_package(Boost REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES
        include/qac/lexer/lexer.h
        include/qac/parser/parser.h
        include/qac/generator/generator.h
        include/qac/generator/html-generator.h
)

set(SOURCE_FILES
        src/main.cpp
        src/lexer/lexer.cpp
        src/parser/parser.cpp
        src/generator/generator.cpp
        src/generator/html-generator.cpp
)

include_directories(include ${Boost_INCLUDE_DIRS})
add_executable(qac ${SOURCE_FILES} ${HEADER_FILES})
