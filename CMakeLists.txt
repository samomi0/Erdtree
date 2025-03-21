cmake_minimum_required(VERSION 3.20)

project(Erdtree)

set(CMAKE_CXX_STANDARD 20)

find_package(yaml-cpp REQUIRED)
find_package(spdlog)

add_subdirectory(${CMAKE_SOURCE_DIR}/example)

add_executable(Erdtree 
    src/main.cpp
    src/ConfigManager.cpp
    src/SharedMemoryManager.cpp
    src/ProcessManager.cpp
    src/SignalManager.cpp
    src/HookManager.cpp
    src/DaemonController.cpp
)

target_include_directories(Erdtree PRIVATE
    include
    ${yaml-cpp_INCLUDE_DIR}
)
target_link_libraries(Erdtree yaml-cpp::yaml-cpp spdlog::spdlog)