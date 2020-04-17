cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(PONG)

set(CXX_FLAGS, "-Wall -std=c++17 -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")



set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src src/include)

file(GLOB SRCS src/*.cpp) #src/*.h

add_executable(pong ${SRCS})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(pong ${SDL2_LIBRARIES})
