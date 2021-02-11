#pragma once

#include "gtest/gtest.h"

#include <lib_coffee_machine/coffee_machine.h>
#include <lib_coffee_machine/default_configuration.h>

#include "mock/mock_controller.h"
#include "mock/mock_display.h"
#include "mock/mock_io_pin.h"
#include "mock/mock_sensor.h"
#include "mock/mock_serial.h"
#include "mock/test_adapter.h"

using Adapter = TestAdapter;
using Configuration = DefaultConfiguration;

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
        machine = new CoffeeMachine<Adapter, Configuration>(
            &controller, &serial, &mode_switch_pin, &display, &heater_pin, &water_sensor,
            &steam_sensor);
        creation_time = Adapter::millis();

        set_default_mock_environment();

        time_step();
    }

    void TearDown() override
    {
        delete machine;
        machine = nullptr;
    }

    /**
     * @brief Set the default mock environment
     *
     */
    void set_default_mock_environment()
    {
        // pin_status true means the button is not pressed
        mode_switch_pin.pin_status = true;
        // Healthy water sensor and low temperature
        water_sensor.temp_c = 10.0f - Configuration::WATER_TEMP_OFFSET;
        water_sensor.healthy = true;
        // Healthy steam sensor and low temperature
        steam_sensor.temp_c = 10.0f - Configuration::STEAM_TEMP_OFFSET;
        steam_sensor.healthy = true;
        // Healthy controller returning the max pwm value (= relay on)
        controller.mock_output = controller.max_output;
        controller.healthy = true;
        // Serial without inputs available
        serial.input_available = false;
    }

    /**
     * @brief Simulate the time passing by adding 500 ms to the return value of
     * Adapter::millis()
     */
    void time_step()
    {
        Adapter::millis_ret += 500;
    }

    MockDisplay display;
    MockSerial serial;
    MockController controller;
    MockIOPin mode_switch_pin;
    MockIOPin heater_pin;
    MockSensor water_sensor;
    MockSensor steam_sensor;
    CoffeeMachine<Adapter, Configuration> *machine = nullptr;

    unsigned long creation_time;
};