#include "common_test.h"

class TestSerialInterface : public CommonTest
{
};

TEST_F(TestSerialInterface, testSerialEnableAndDisableOutputStatus)
{
    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Machine::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
    // Verify that nothing was sent out on the serial output
    ASSERT_EQ(strlen(serial.output_string), 0);

    // Mock the input to enable the serial output
    serial.set_mock_input("output on");

    status = machine->spin();

    // Verify the status request to send
    ASSERT_EQ(status.machine_mode, Machine::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
    // Verify message requested as output
    ASSERT_EQ(strcmp(serial.output_string, "0,10.0,95.0,1,Heating..."), 0);
}

TEST_F(TestSerialInterface, testSerialDebugMode)
{
    // Mock the input to enable the serial output
    serial.set_mock_input("output on");

    auto status = machine->spin();

    // Enable debug mode to verify the returned temperature is the mock one
    serial.set_mock_input("debug on");

    status = machine->spin();

    // Set the mock temperature
    serial.set_mock_input("temp 37");

    status = machine->spin();

    // The serial output is paced so we need to mock time passing
    time_step();

    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Machine::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 37.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Debug mode"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
    // Verify message requested as output
    ASSERT_EQ(strcmp(serial.output_string, "0,37.0,95.0,1,Debug mode"), 0);

    // Change to STEAM mode to verify the status is updated
    mode_switch_pin.set_steam_mode();
    auto steam_mode_ts = Adapter::millis_ret;

    // The serial output is paced so we need to mock time passing
    time_step();

    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Machine::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 37.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Debug mode"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, steam_mode_ts);
    // Verify message requested as output
    ASSERT_EQ(strcmp(serial.output_string, "1,37.0,145.0,1,Debug mode"), 0);
}
