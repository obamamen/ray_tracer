cmake_minimum_required(VERSION 3.29)
project(ray_tracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

add_executable(rt main.cpp ${SOURCES} ${HEADERS})
