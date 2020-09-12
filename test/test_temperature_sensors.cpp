#include "gtest/gtest.h"

#include <coffee_machine/CoffeeMachine.h>
#include <coffee_machine/Configuration.h>

#include "mock/CppAdapter.h"
#include "mock/MockDisplay.h"
#include "mock/MockHeater.h"
#include "mock/MockModeDetector.h"
#include "mock/MockPID.h"
#include "mock/MockSerial.h"
#include "mock/MockTempSensor.h"

using Adapter = CppAdapter;

class TestGaggiaCoffeeMachine : public ::testing::Test
{
  protected:
    TestGaggiaCoffeeMachine()
        : pid(), serial(), mode_detector(), display(), heater(), water_sensor("water"),
          steam_sensor("steam"), creation_time(Adapter::millis())
    {
    }

    void SetUp() override
    {
        display.reset();
        serial.reset();
        pid.reset();
        mode_detector.reset();
        heater.reset();
        water_sensor.reset();
        steam_sensor.reset();
        creation_time = Adapter::millis();
    }

    CoffeeMachine<Adapter> make_machine()
    {
        return CoffeeMachine<Adapter>(&pid, &serial, &mode_detector, &display, &heater,
                                      &water_sensor, &steam_sensor);
    }

    unsigned long normalize_time(const double &time)
    {
        return time - creation_time;
    }

    MockDisplay<Adapter> display;
    MockSerial<Adapter> serial;
    MockPID pid;
    MockModeDetector mode_detector;
    MockHeater heater;
    MockTempSensor<Adapter> water_sensor;
    MockTempSensor<Adapter> steam_sensor;

    unsigned long creation_time;
};

TEST_F(TestGaggiaCoffeeMachine, testWaterModeHealthySensorHeating)
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
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testWaterModeHealthySensorCooling)
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
    ASSERT_EQ(status.status_message, "Cooling...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testWaterModeHealthySensorStandby)
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
    ASSERT_EQ(status.status_message, "Ready");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testWaterModeFaultySensor)
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
    ASSERT_EQ(status.status_message, "Unable to read temperature from sensor: water");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testSteamModeHealthySensorHeating)
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
    ASSERT_EQ(status.status_message, "Heating...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testSteamModeHealthySensorCooling)
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
    ASSERT_EQ(status.status_message, "Cooling...");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testSteamModeHealthySensorStandby)
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
    ASSERT_EQ(status.status_message, "Ready");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

TEST_F(TestGaggiaCoffeeMachine, testSteamModeFaultySensor)
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
    ASSERT_EQ(status.status_message, "Unable to read temperature from sensor: steam");
    ASSERT_EQ(normalize_time(status.time_since_start), 0.0);
    ASSERT_EQ(normalize_time(status.time_since_steam_mode), 0.0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
