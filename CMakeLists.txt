cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
project(PONG)

#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})
add_definitions(${SDL2_DEFINITIONS})

file(GLOB SRCS src/*.cpp) 

add_executable(pong ${SRCS})
#string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(pong pthread ${SDL2_LIBRARIES})
