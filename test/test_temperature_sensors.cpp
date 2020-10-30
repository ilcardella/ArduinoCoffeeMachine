#include "CommonTest.h"

class TestTemperatureSensors : public CommonTest
{
};

TEST_F(TestTemperatureSensors, testWaterModeHealthySensorHeating)
{
    // Mock healthy temp sensor and pid controllerand a "low" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    pid.relay = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Heating...").c_str()), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testWaterModeHealthySensorCooling)
{
    // Mock healthy temp sensor and pid controller and a "high" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = 100.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    pid.relay = false;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 100.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Cooling...").c_str()), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testWaterModeHealthySensorStandby)
{
    // Mock healthy temp sensor and pid controller and a "target" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = Configuration::TARGET_WATER_TEMP;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    pid.relay = false;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Ready").c_str()), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testWaterModeFaultySensor)
{
    // Mock healthy temp sensor and pid controller and a "target" temperature
    mode_detector.mode = Gaggia::Mode::WATER_MODE;
    water_sensor.temp_c = Configuration::TARGET_WATER_TEMP;
    water_sensor.healthy = false;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    // Set this to true to verify that it does not influence the heater status
    pid.relay = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 0.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(
        strcmp(status.status_message, std::string("Temperature sensor fault").c_str()),
        0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeHealthySensorHeating)
{
    // Mock healthy temp sensor and pid controllerand a "low" temperature
    mode_detector.mode = Gaggia::Mode::STEAM_MODE;
    steam_sensor.temp_c = 10.0f;
    steam_sensor.healthy = true;
    // Water sensor should not be used in this mode
    water_sensor.temp_c = 0.0f;
    water_sensor.healthy = false;
    pid.relay = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Heating...").c_str()), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeHealthySensorCooling)
{
    // Mock healthy temp sensor and pid controller and a "high" temperature
    mode_detector.mode = Gaggia::Mode::STEAM_MODE;
    steam_sensor.temp_c = 200.0f;
    steam_sensor.healthy = true;
    // Water sensor should not be used in this mode
    water_sensor.temp_c = 0.0f;
    water_sensor.healthy = false;
    pid.relay = false;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 200.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Cooling...").c_str()), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeHealthySensorStandby)
{
    // Mock healthy temp sensor and pid controller and a "target" temperature
    mode_detector.mode = Gaggia::Mode::STEAM_MODE;
    steam_sensor.temp_c = Configuration::TARGET_STEAM_TEMP;
    steam_sensor.healthy = true;
    // Water sensor should not be used in this mode
    water_sensor.temp_c = 0.0f;
    water_sensor.healthy = false;
    pid.relay = false;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Ready").c_str()), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeFaultySensor)
{
    // Mock healthy temp sensor and pid controller and a "target" temperature
    mode_detector.mode = Gaggia::Mode::STEAM_MODE;
    steam_sensor.temp_c = Configuration::TARGET_STEAM_TEMP;
    steam_sensor.healthy = false;
    // Water sensor should not be used in this mode
    water_sensor.temp_c = 0.0f;
    water_sensor.healthy = false;
    // Set this to true to verify that it does not influence the heater status
    pid.relay = true;

    auto machine = make_machine();
    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 0.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(
        strcmp(status.status_message, std::string("Temperature sensor fault").c_str()),
        0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
