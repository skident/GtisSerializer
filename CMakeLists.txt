cmake_minimum_required(VERSION 3.1)
project(BhrSerializer C CXX)

# define the project
set (CMAKE_CXX_STANDARD 17)

# which directories will be include to project.
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third/magic_get/include)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third/nlohmann/include)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third/bhrSQL/include)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
#include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/BhrSerializable/include)

# Find source files
#file(GLOB_RECURSE INCLUDES include/*.h)
#file(GLOB_RECURSE INCLUDES src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp include/*.h src/*.h)

add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} INTERFACE bhrSQL)

#target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/BhrSerializable/include)
#target_link_libraries(${PROJECT_NAME} INTERFACE BhrSerializable)
