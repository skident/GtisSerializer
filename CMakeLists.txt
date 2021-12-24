cmake_minimum_required(VERSION 3.1)
project(eos_cpp)

# define the project
set (CMAKE_CXX_STANDARD 17)

# which directories will be include to project.
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third/magic_get/include)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

# Find source files
file(GLOB_RECURSE INCLUDES include/*.h *.cpp)
file(GLOB_RECURSE SRC_INCLUDES src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)


add_executable(${PROJECT_NAME} ${INCLUDES} ${SRC_INCLUDES} ${SOURCES})

#target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/eos_cpp/include)
#target_link_libraries(${PROJECT_NAME} INTERFACE eos_cpp)
