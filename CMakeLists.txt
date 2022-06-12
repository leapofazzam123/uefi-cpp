cmake_minimum_required(VERSION 3.15)

project(uefi-cpp
    VERSION 0.1.0
    DESCRIPTION "A simple C++ library for interfacing with UEFI"
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
