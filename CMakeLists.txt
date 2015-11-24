cmake_minimum_required(VERSION 3.3)
project(qac VERSION 0.2.2 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost REQUIRED)
find_package(GFlags REQUIRED)
find_package(Glog REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(HEADER_FILES
    include/qac/lexer/lexer.h
    include/qac/parser/parser.h
    include/qac/parser/cst_nodes.h
    include/qac/parser/ast_nodes.h
    include/qac/parser/cst_to_ast_visitor.h
    include/qac/parser/ast_render_visitor.h
    include/qac/generator/generator.h
    include/qac/generator/html-generator.h
    include/qac/generator/anki-generator.h
)

set(COMMON_SOURCE_FILES
    src/flags.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/parser/cst_to_ast_visitor.cpp
    src/parser/ast_render_visitor.cpp
    src/parser/ast_nodes.cpp
    src/generator/generator.cpp
    src/generator/html-generator.cpp
    src/generator/anki-generator.cpp
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

set(GENERATED_DIR ${PROJECT_BINARY_DIR}/generated)
configure_file(${PROJECT_SOURCE_DIR}/qac_config.h.in ${GENERATED_DIR}/qac_config.h)

include_directories(include ${Boost_INCLUDE_DIRS} ${GFLAGS_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR} ${GENERATED_DIR})

add_executable(qac ${QAC_SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(qac ${GFLAGS_LIBRARY} ${GLOG_LIBRARY})

add_executable(qac_test ${TEST_SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(qac_test ${GFLAGS_LIBRARY} ${GLOG_LIBRARY})

install(TARGETS qac DESTINATION bin)
