
add_executable(main main.cpp)

add_library(Container container.cpp)
target_link_libraries (main LINK_PUBLIC Container)