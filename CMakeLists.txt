cmake_minimum_required(VERSION 3.2)
project(bachero)

set(CMAKE_CXX_STANDARD 17)

#----------------------INCLUDE---------------------------#

include_directories(
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/libs"
)

#------------------------LIBS----------------------------#

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(libs)

#-----------------------SOURCE---------------------------#

file(GLOB_RECURSE HEADERS
        "${PROJECT_SOURCE_DIR}/include/*.h")

file(GLOB_RECURSE SOURCE
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp")

#----------------------TARGETS---------------------------#

add_executable(bachero main.cpp ${HEADERS} ${SOURCE})
target_link_libraries(bachero ${SDL2_LIBRARIES})