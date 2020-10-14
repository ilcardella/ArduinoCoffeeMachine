#include "CommonTest.h"

class TestSerialInterface : public CommonTest
{
};

TEST_F(TestSerialInterface, testSerialReadsInputsAndSendStatus)
{
    // Mock healthy temp sensor, pid controller and a "low" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    pid.relay = true;
    pid.healthy = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    // Verify the read_input() is called
    ASSERT_TRUE(serial.read_input_called);
    serial.read_input_called = false; // reset

    status = machine.spin();
    // Verify the read_input() is called
    ASSERT_TRUE(serial.read_input_called);

    // Verify the status request to send is correct and the same as returned by the spin()
    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
    ASSERT_EQ(serial.status_to_send.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(serial.status_to_send.current_temperature, 10.0);
    ASSERT_EQ(serial.status_to_send.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(serial.status_to_send.water_heater_on);
    ASSERT_EQ(serial.status_to_send.status_message, "Heating...");
    ASSERT_EQ(normalize_time(serial.status_to_send.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(serial.status_to_send.time_since_steam_mode), 0.0);
}

TEST_F(TestSerialInterface, testSerialDebugMode)
{
    // Mock healthy temp sensor, pid controller and a "low" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    pid.relay = true;
    pid.healthy = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    // Enable debug mode and verify the returned temperature is the mock one
    serial.debug_on = true;
    // Sensor should not be used anyway in debug mode
    water_sensor.healthy = false;

    status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 42.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Debug mode");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    mode_detector.mode = Gaggia::Mode::STEAM_MODE;
    status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 42.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Debug mode");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestSerialInterface, testSerialPIDGainsInputs)
{
    // Mock healthy temp sensor, pid controller and a "low" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    pid.relay = true;
    pid.healthy = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    // Simulate PID gains coming from serial interface
    serial.kp_in = 31.0;
    serial.ki_in = 32.0;
    serial.kd_in = 32.0;
    status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
    ASSERT_EQ(serial.kp_in, pid.kp);
    ASSERT_EQ(serial.ki_in, pid.ki);
    ASSERT_EQ(serial.kd_in, pid.kd);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
