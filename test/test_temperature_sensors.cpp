#include "common_test.h"

class TestTemperatureSensors : public CommonTest
{
};

TEST_F(TestTemperatureSensors, testWaterModeHealthySensorHeating)
{
    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Heating...").c_str()), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
}

TEST_F(TestTemperatureSensors, testWaterModeHealthySensorCooling)
{
    // Mock an high water temperature
    water_sensor.temp_c = 100.0f - Configuration::WATER_TEMP_OFFSET;
    // Mock the controller requesting the relay off
    controller.set_min_output();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 100.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Cooling...").c_str()), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
}

TEST_F(TestTemperatureSensors, testWaterModeHealthySensorStandby)
{
    // Mock the current water temp just on target
    water_sensor.temp_c =
        Configuration::TARGET_WATER_TEMP - Configuration::WATER_TEMP_OFFSET;
    // Mock the controller requesting the relay off
    controller.set_min_output();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Ready").c_str()), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
}

TEST_F(TestTemperatureSensors, testWaterModeFaultySensor)
{
    // Mock the current water temp just on target
    water_sensor.temp_c =
        Configuration::TARGET_WATER_TEMP - Configuration::WATER_TEMP_OFFSET;
    water_sensor.healthy = false;
    // Set this to output ON to verify that it does not influence the heater status
    controller.set_max_output();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 0.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(
        strcmp(status.status_message, std::string("Temperature sensor fault").c_str()),
        0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
}

TEST_F(TestTemperatureSensors, testSteamModeHealthySensorHeating)
{
    // Mock the mode button as pressed
    mode_switch_pin.set_steam_mode();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Heating...").c_str()), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeHealthySensorCooling)
{
    // Mock the mode button as pressed
    mode_switch_pin.set_steam_mode();
    // Mock a high steam temperature
    steam_sensor.temp_c = 200.0f - Configuration::STEAM_TEMP_OFFSET;
    // controller requesting the relay as off
    controller.set_min_output();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 200.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Cooling...").c_str()), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeHealthySensorStandby)
{
    // Mock the mode button as pressed
    mode_switch_pin.set_steam_mode();
    // Mock a high steam temperature
    steam_sensor.temp_c =
        Configuration::TARGET_STEAM_TEMP - Configuration::STEAM_TEMP_OFFSET;
    // controller requesting the relay as off
    controller.set_min_output();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, std::string("Ready").c_str()), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, 0.0);
}

TEST_F(TestTemperatureSensors, testSteamModeFaultySensor)
{
    // Mock the mode button as pressed
    mode_switch_pin.set_steam_mode();
    // Mock a high steam temperature
    steam_sensor.temp_c =
        Configuration::TARGET_STEAM_TEMP - Configuration::STEAM_TEMP_OFFSET;
    steam_sensor.healthy = false;
    // Set this to verify that it does not influence the heater status
    controller.set_max_output();

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 0.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(
        strcmp(status.status_message, std::string("Temperature sensor fault").c_str()),
        0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, 0.0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
