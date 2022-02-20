project(ngram)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

add_executable(ngram ./main.cpp ./txt_parser.cpp)

add_compile_options(-g -Wall)
