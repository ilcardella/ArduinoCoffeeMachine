#include "common_test.h"

class TestPIDController : public CommonTest
{
};

TEST_F(TestPIDController, testFaultyController)
{
    // Force the controller to be faulty
    controller.healthy = false;

    auto status = machine->spin();

    ASSERT_EQ(status.machine_mode, Machine::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_FALSE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "PID fault"), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);

    controller.healthy = true;
    status = machine->spin();

    ASSERT_EQ(status.machine_mode, Machine::Mode::WATER_MODE);
    ASSERT_EQ(status.current_temperature, 10.0);
    ASSERT_EQ(status.target_temperature, Configuration::TARGET_WATER_TEMP);
    ASSERT_TRUE(status.water_heater_on);
    ASSERT_EQ(strcmp(status.status_message, "Heating..."), 0);
    ASSERT_EQ(status.start_timestamp, 0.0);
    ASSERT_EQ(status.steam_mode_timestamp, Adapter::millis_ret);
}
