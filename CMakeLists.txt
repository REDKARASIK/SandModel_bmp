cmake_minimum_required(VERSION 3.5.0)

project(
    SandModel
    VERSION 1.0
    DESCRIPTION "C++ Sand Model Year 2024 Labwork3"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(lib)
add_subdirectory(bin)