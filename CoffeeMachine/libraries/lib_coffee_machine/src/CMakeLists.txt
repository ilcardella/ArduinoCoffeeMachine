cmake_minimum_required(VERSION 3.10)

add_library(${PROJECT_NAME} INTERFACE)
# Not using $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/coffee_machine means that
# the include will be #include "coffee_machine/<file>.h"
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)