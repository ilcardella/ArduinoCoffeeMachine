#pragma once

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

class CommonTest : public ::testing::Test
{
  protected:
    CommonTest()
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