cmake_minimum_required(VERSION 2.8)
project(JSON CXX)

add_library(Task task.cpp)
target_include_directories (Task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cpp)
target_link_libraries (main LINK_PUBLIC Task)
