cmake_minimum_required(VERSION 2.8)

project(WebServer CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

aux_source_directory(. DIR_SRCS)

add_executable(WebServer ${DIR_SRCS})

target_link_libraries(WebServer pthread)