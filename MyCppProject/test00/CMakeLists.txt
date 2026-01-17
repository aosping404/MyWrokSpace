cmake_minimum_required(VERSION 3.10)
project(test00 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test00 src/main.cpp src/greet.cpp)
