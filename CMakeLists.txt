cmake_minimum_required(VERSION 3.19)
project(LoggerLibrary VERSION 0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared library" OFF)

add_library(LoggerLibrary
        src/FileLogger.cpp
        src/SocketLibrary.cpp
)

target_include_directories(LoggerLibrary PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)