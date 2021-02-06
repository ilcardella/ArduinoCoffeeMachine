#pragma once

#include "gtest/gtest.h"

#include <coffee_machine/coffee_machine.h>

#include "mock/mock_controller.h"
#include "mock/mock_display.h"
#include "mock/mock_io_pin.h"
#include "mock/mock_sensor.h"
#include "mock/mock_serial.h"
#include "mock/test_adapter.h"

using Adapter = TestAdapter;

class CommonTest : public ::testing::Test
{
  protected:
    CommonTest()
        : controller(), serial(), mode_switch_pin(), display(), heater_pin(),
          water_sensor(), steam_sensor(), creation_time(Adapter::millis())
    {
    }

    void SetUp() override
    {
        serial.reset();
        controller.reset();
        mode_switch_pin.reset();
        heater_pin.reset();
        water_sensor.reset();
        steam_sensor.reset();
        creation_time = Adapter::millis();
    }

    CoffeeMachine<Adapter> make_machine()
    {
        return CoffeeMachine<Adapter>(&controller, &serial, &mode_switch_pin, &display,
                                      &heater_pin, &water_sensor, &steam_sensor);
    }

    unsigned long normalize_time(const double &time)
    {
        return time - creation_time;
    }

    MockDisplay display;
    MockSerial serial;
    MockController controller;
    MockIOPin mode_switch_pin;
    MockIOPin heater_pin;
    MockSensor water_sensor;
    MockSensor steam_sensor;

    unsigned long creation_time;
};