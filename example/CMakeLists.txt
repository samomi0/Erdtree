cmake_minimum_required(VERSION 3.20)

project(DummyFunc)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog)

add_executable(DummyFunc DummyFunc.cpp)

target_include_directories(DummyFunc PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(DummyFunc spdlog::spdlog)