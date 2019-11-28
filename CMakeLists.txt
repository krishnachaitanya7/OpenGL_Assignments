cmake_minimum_required(VERSION 3.14)
project(opengl_assignments)

set(CMAKE_CXX_STANDARD 14)
#include_directories(homework_2)
#include_directories(homework_3)
#include_directories(homework_4)
link_directories(${CMAKE_SOURCE_DIR}/final_project)
add_subdirectory(final_project)

link_directories(${CMAKE_SOURCE_DIR}/experiments)
add_subdirectory(experiments)

#include(homework_0/CMakeLists.txt)
#include(homework_2/CMakeLists.txt)
#include(homework_3/CMakeLists.txt)
#include(homework_4/CMakeLists.txt)
#include(homework_5/CMakeLists.txt)
