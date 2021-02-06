#include "common_test.h"

class TestSerialInterface : public CommonTest
{
};

TEST_F(TestSerialInterface, testSerialReadsInputsAndSendStatus)
{
    auto machine = make_machine();

    // Mock healthy temp sensor, pid controller and a "low" temperature
    mode_switch_pin.pin_status = true;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    controller.mock_output = controller.max_output;
    controller.healthy = true;

    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    // Verify the read_input() is called
    ASSERT_TRUE(serial.read_input_called);
    serial.read_input_called = false; // reset

    status = machine.spin();
    // Verify the read_input() is called
    ASSERT_TRUE(serial.read_input_called);

    // Verify the status request to send
    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
    // Verify message requested as output
    ASSERT_EQ(strcmp(serial.output_string, "0,10.0,95.0,1,Heating..."), 0);
}

TEST_F(TestSerialInterface, testSerialDebugMode)
{
    auto machine = make_machine();

    // Mock healthy temp sensor, pid controller and a "low" temperature
    mode_switch_pin.pin_status = true;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    controller.mock_output = controller.max_output;
    controller.healthy = true;

    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    // Enable debug mode to verify the returned temperature is the mock one
    strcpy(serial.input_string, "debug on");
    // Sensor should not be used anyway in debug mode
    water_sensor.healthy = false;

    status = machine.spin();

    // Set the mock temperature
    strcpy(serial.input_string, "temp 37.0");
    status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 37.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Debug mode"), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
    // Verify message requested as output
    ASSERT_EQ(strcmp(serial.output_string, "0,37.0,95.0,1,Heating..."), 0);

    mode_switch_pin.pin_status = false;
    status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 37.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Debug mode"), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
    // Verify message requested as output
    ASSERT_EQ(strcmp(serial.output_string, "1,37.0,145.0,1,Heating..."), 0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
