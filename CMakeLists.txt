cmake_minimum_required(VERSION 3.19)
project(LoggerLibrary VERSION 0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ======================================
# ==== Сборка библиотеки и клиента =====
# ======================================

option(BUILD_SHARED_LIBS "Build shared library" OFF)

add_library(LoggerLibrary
        src/FileLogger.cpp
        src/SocketLibrary.cpp
)

target_compile_options(LoggerLibrary PRIVATE --coverage)
target_link_options(LoggerLibrary PRIVATE --coverage)

target_include_directories(LoggerLibrary PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(Program
    main.cpp
    Program/Program.cpp
    include/Program/Program.h
    include/ThreadClasses/ThreadClasses.h
    ThreadClasses/ThreadClasses.cpp
)

target_compile_options(Program PRIVATE --coverage)
target_link_options(Program PRIVATE --coverage)

target_include_directories(Program PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(Program PRIVATE
    LoggerLibrary
)

# ============================
# ====== Сборка сервера ======
# ============================

add_executable(Server
    server_main.cpp
    Server/Server.cpp
    include/Server/Server.h
)

target_include_directories(Server PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(Server PRIVATE --coverage)
target_link_options(Server PRIVATE --coverage)

# ============================
# === Тестирование с GTest ===
# ============================

find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing()

add_executable(TestLogLibrary
    tests/FileLoggerTests.cpp
    tests/ThreadClassesTests.cpp
    ThreadClasses/ThreadClasses.cpp
)

target_link_libraries(TestLogLibrary PRIVATE
    LoggerLibrary
    GTest::gtest_main
)

gtest_discover_tests(TestLogLibrary)

target_compile_options(TestLogLibrary PRIVATE --coverage)
target_link_options(TestLogLibrary PRIVATE --coverage)
