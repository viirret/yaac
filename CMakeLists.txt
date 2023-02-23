cmake_minimum_required(VERSION 3.0)
project(yaac)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB SRC_FILES "${SRC_DIR}/*.cc")

add_executable(x ${SRC_FILES})

target_link_libraries(x -lSDL2 -lSDL2_ttf -lSDL2_mixer)

