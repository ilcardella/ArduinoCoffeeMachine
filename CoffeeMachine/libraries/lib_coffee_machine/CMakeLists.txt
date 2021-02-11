cmake_minimum_required(VERSION 3.10)

# set the project name
project(lib_coffee_machine VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the library
add_subdirectory(src)

# Enable testing only when building this project directly
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(PACKAGE_TESTS "Build the tests" ON)
endif()
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
