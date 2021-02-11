cmake_minimum_required(VERSION 3.10)

macro(custom_add_gtest TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} ${PROJECT_NAME} gtest gmock gtest_main)
    gtest_discover_tests(${TEST_NAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()


# Add GoogleTest
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")
# This keeps the cache cleaner
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

custom_add_gtest("test_moving_average")
custom_add_gtest("test_temperature_sensors")
custom_add_gtest("test_temperature_controller")
custom_add_gtest("test_safety_timeouts")
custom_add_gtest("test_serial_interface")
