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

set(COMMON_SOURCE_FILES
        src/lexer/lexer.cpp
        src/parser/parser.cpp
        src/generator/generator.cpp
        src/generator/html-generator.cpp
)

set(QAC_SOURCE_FILES
        src/main.cpp
        ${COMMON_SOURCE_FILES}
)

set(TEST_SOURCE_FILES
    test/catch.hpp
    test/test.cpp
    test/parser_test.cpp
    test/lexer_test.cpp
    ${COMMON_SOURCE_FILES}
)

include_directories(include ${Boost_INCLUDE_DIRS})
add_executable(qac ${QAC_SOURCE_FILES} ${HEADER_FILES})
add_executable(qac_test ${TEST_SOURCE_FILES} ${HEADER_FILES})
