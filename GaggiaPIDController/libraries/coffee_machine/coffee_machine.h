#pragma once

#include "common.h"
#include "configuration.h"
#include "display.h"
#include "heater.h"
#include "interfaces.h"
#include "mode_detector.h"
#include "relay_controller.h"
#include "serial_interface.h"
#include "temperature_sensor.h"

template <class Adapter> class CoffeeMachine
{
  public:
    CoffeeMachine(Controller *controller, BaseSerialInterface *serial,
                  IOPin *mode_switch_pin, BaseDisplay *display_type, IOPin *heater_pin,
                  BaseSensor *water_sensor, BaseSensor *steam_sensor)
        : temp_controller(controller), serial(serial), mode_detector(mode_switch_pin),
          display(display_type), heater(heater_pin),
          water_t_sensor("water_sensor", water_sensor,
                         Configuration::WATER_TEMP_REFRESH_PERIOD, 10),
          steam_t_sensor("steam_sensor", steam_sensor,
                         Configuration::STEAM_TEMP_REFRESH_PERIOD, 10),
          machine_status()
    {
        // Mark machine start time
        machine_status.time_since_start = Adapter::millis();
        machine_status.time_since_steam_mode = Adapter::millis();
    }

    Gaggia::ControlStatus spin()
    {
        serial.read_input();

        if (update_machine_status())
        {
            update_pid();
        }

        heater.set(machine_status.water_heater_on);

        display.update(machine_status);

        serial.print_status(machine_status);

        return machine_status;
    }

  private:
    bool update_machine_status()
    {
        unsigned long now = Adapter::millis();

        // Set SSR off by default and let the PID decide whether to turn it on
        machine_status.water_heater_on = false;
        // Read operation mode
        machine_status.machine_mode = mode_detector.get_mode();

        // Reset steam mode timeout counter when not in steam mode
        if (machine_status.machine_mode != Gaggia::Mode::STEAM_MODE)
        {
            machine_status.time_since_steam_mode = now;
        }

        // Set target temperature based on machine mode
        machine_status.target_temperature =
            (machine_status.machine_mode == Gaggia::Mode::WATER_MODE)
                ? Configuration::TARGET_WATER_TEMP
                : Configuration::TARGET_STEAM_TEMP;

        // When debug mode is enabled do not read sensors
        if (serial.is_debug_active())
        {
            machine_status.current_temperature = serial.get_mock_temperature();
            strncpy(machine_status.status_message, string_utils::strings::DEBUG_MODE,
                    machine_status.MSG_LEN);
            return true;
        }

        // Select correct sensor for current operation mode
        TemperatureSensor<Adapter> *sensor =
            (machine_status.machine_mode == Gaggia::Mode::WATER_MODE) ? &water_t_sensor
                                                                      : &steam_t_sensor;

        // Get the current temp from the temperature sensor
        float sensor_value;
        if (not sensor || not sensor->get_temperature_celsius(&sensor_value))
        {
            strncpy(machine_status.status_message,
                    string_utils::strings::TEMP_SENSOR_ERROR, machine_status.MSG_LEN);
            return false;
        }

        // Add a static offset due possible reading error
        sensor_value += (machine_status.machine_mode == Gaggia::Mode::WATER_MODE)
                            ? Configuration::WATER_TEMP_OFFSET
                            : Configuration::STEAM_TEMP_OFFSET;
        machine_status.current_temperature = static_cast<double>(sensor_value);

        // Use a tolerance of +/- 1Deg for the message
        double diff =
            machine_status.target_temperature - machine_status.current_temperature;
        if (diff < -1.0)
        {
            strncpy(machine_status.status_message, string_utils::strings::COOLING,
                    machine_status.MSG_LEN);
        }
        else if (diff > 1.0)
        {
            strncpy(machine_status.status_message, string_utils::strings::HEATING,
                    machine_status.MSG_LEN);
        }
        else
        {
            strncpy(machine_status.status_message, string_utils::strings::READY,
                    machine_status.MSG_LEN);
        }

        // If the machine has been on for more than the safety limit, then report a
        // problem so the heater will be turned off
        if (Configuration::SAFETY_TIMEOUT > 0 &&
            (now - machine_status.time_since_start) > Configuration::SAFETY_TIMEOUT)
        {
            strncpy(machine_status.status_message,
                    string_utils::strings::SAFETY_TIMEOUT_EXPIRED,
                    machine_status.MSG_LEN);
            return false;
        }
        // Check steam mode timeout to avoid keeping the machine at high temps for
        // long
        if (Configuration::STEAM_TIMEOUT > 0 &&
            (now - machine_status.time_since_steam_mode) > Configuration::STEAM_TIMEOUT)
        {
            strncpy(machine_status.status_message,
                    string_utils::strings::STEAM_TIMEOUT_EXPIRED, machine_status.MSG_LEN);
            return false;
        }
        return true;
    }

    void update_pid()
    {
        if (not temp_controller.compute(machine_status.current_temperature,
                                        machine_status.target_temperature,
                                        &(machine_status.water_heater_on)))
        {
            machine_status.water_heater_on = false;
            strncpy(machine_status.status_message, string_utils::strings::PID_FAULT,
                    machine_status.MSG_LEN);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////

    TemperatureSensor<Adapter> water_t_sensor;
    TemperatureSensor<Adapter> steam_t_sensor;
    RelayController<Adapter> temp_controller;
    SerialInterface<Adapter> serial;
    Display<Adapter> display;
    ModeDetector mode_detector;
    Gaggia::ControlStatus machine_status;
    Heater heater;
};
