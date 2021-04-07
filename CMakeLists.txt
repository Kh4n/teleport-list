cmake_minimum_required(VERSION 3.10)
project(teleport-list)
set (CMAKE_CXX_STANDARD 11)

message("Build type: ${CMAKE_BUILD_TYPE}")

add_executable(benchmark "Benchmark.cpp")
target_include_directories(benchmark PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")