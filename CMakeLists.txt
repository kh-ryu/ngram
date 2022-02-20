project(ngram)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

add_executable(ngram src/main.cpp src/txt_parser.cpp src/analyzer.cpp src/argument_parser.cpp)
add_compile_options(CMAKE_BUILD_TYPE Release)

add_compile_options(-g -Wall)
