#include "common_test.h"

class TestPIDController : public CommonTest
{
};

TEST_F(TestPIDController, testFaultyPIDController)
{
    auto machine = make_machine();

    // Mock healthy temp sensor, pid controller and a "low" temperature
    mode_switch_pin.pin_status = true;
    water_sensor.temp_c = 10.0f;
    water_sensor.healthy = true;
    // Steam sensor should not be used in this mode
    steam_sensor.temp_c = 0.0f;
    steam_sensor.healthy = false;
    // Force the PID controller as faulty
    controller.mock_output = controller.max_output;
    controller.healthy = false;

    auto status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "PID fault"), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);

    controller.healthy = true;
    status = machine.spin();

    ASSERT_EQ(status.machine_mode, Gaggia::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
