cmake_minimum_required(VERSION 3.10)

# set the project name
project(GaggiaPIDController VERSION 1.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the library
add_subdirectory(GaggiaPIDController/libraries)

# add test
add_subdirectory(test)
