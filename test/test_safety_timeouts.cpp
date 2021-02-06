#include "common_test.h"

class TestSafetyTimeouts : public CommonTest
{
};

TEST_F(TestSafetyTimeouts, testWaterSafetyTimeout)
{
    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);

    // Simulate getting at the limit of the safety timeout
    unsigned long exceeded_time = Configuration::SAFETY_TIMEOUT;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, exceeded_time);

    // Simulate exceeding the safety timeout of 1 second
    exceeded_time += 1000;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Safety timeout expired"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, exceeded_time);

    // Change to STEAM mode and verify the safety measures are still in effect
    mode_switch_pin.set_steam_mode();

    exceeded_time += 1000;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Safety timeout expired"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    // -1000 because the mode changed to steam 1 second ago
    ASSERT_EQ(status.steam_mode_timestamp, exceeded_time - 1000);

    // Change back to WATER mode and verify again
    mode_switch_pin.set_water_mode();

    exceeded_time += 1000;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Safety timeout expired"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, exceeded_time);
}

TEST_F(TestSafetyTimeouts, testSteamSafetyTimeout)
{
    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);

    // Change mode to STEAM
    auto steam_mode_ts = Adapter::millis_ret;
    mode_switch_pin.set_steam_mode();

    // Simulate getting at the limit of the STEAM safety timeout
    auto exceeded_time = Configuration::STEAM_TIMEOUT;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, steam_mode_ts);

    // Exceed the steam timeout by one second
    exceeded_time += 1000;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    // Verify the heater is turned off
    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Steam mode timeout expired"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    // 0.0 because last time in water mode was at creation
    ASSERT_EQ(status.steam_mode_timestamp, steam_mode_ts);

    // Change back to WATER mode and verify the timeout is reset
    mode_switch_pin.set_water_mode();

    exceeded_time += 1000;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, exceeded_time);

    // Change mode to STEAM again and verify it's working ok
    mode_switch_pin.set_steam_mode();

    exceeded_time += 1000;
    Adapter::millis_ret = exceeded_time;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::STEAM_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_STEAM_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, exceeded_time - 1000);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
