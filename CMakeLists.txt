cmake_minimum_required(VERSION 3.5)

project(opengl-es-test)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/CMake")

find_package(SDL2 REQUIRED)
find_package(GLESv2 REQUIRED)

file(GLOB SRCS *.cpp engine/*.cpp modules/*.cpp utils/*.cpp)

add_executable(opengl-es-test ${SRCS})
target_include_directories(opengl-es-test PRIVATE . ${SDL2_INCLUDE_DIR})
target_link_libraries(opengl-es-test PRIVATE ${SDL2_LIBRARY} ${GLESv2_LIBRARIES})
