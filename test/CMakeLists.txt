cmake_minimum_required(VERSION 3.10)

macro(gaggia_add_gtest TEST_NAME)
  add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
  target_link_libraries(${TEST_NAME} PUBLIC CoffeeMachineLib gtest)
  gtest_discover_tests(${TEST_NAME})
endmacro()


# Set up GoogleTest
add_subdirectory(third_party/googletest)
target_compile_definitions(gtest
  PUBLIC
    GTEST_LANG_CXX20
    GTEST_HAS_TR1_TUPLE=0
)

enable_testing()
include(GoogleTest)

gaggia_add_gtest("test_temperature_sensors")
gaggia_add_gtest("test_pid_controller")
gaggia_add_gtest("test_safety_timeouts")
