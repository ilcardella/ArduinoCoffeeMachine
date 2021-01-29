#pragma once

#include "BaseTypes.h"
#include "Common.h"
#include "Configuration.h"
#include "Display.h"
#include "SerialInterface.h"

template <class Adapter> class CoffeeMachine
{
  public:
    CoffeeMachine(BasePIDController *pid, BaseSerialInterface *serial,
                  BaseModeDetector *mode_detector, BaseDisplay *display_type,
                  BaseHeater *heater, BaseTemperatureSensor *water_sensor,
                  BaseTemperatureSensor *steam_sensor)
        : pid(pid), serial(serial), mode_detector(mode_detector), display(display_type),
          heater(heater), water_sensor(water_sensor), steam_sensor(steam_sensor),
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

        heater->set(machine_status.water_heater_on);

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
        machine_status.machine_mode = mode_detector->get_mode();

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
        BaseTemperatureSensor *sensor =
            (machine_status.machine_mode == Gaggia::Mode::WATER_MODE) ? water_sensor
                                                                      : steam_sensor;
        // Get the current temp from the temperature sensor
        float sensor_value;
        if (not sensor || not sensor->get_temperature_celsius(&sensor_value))
        {
            strncpy(machine_status.status_message,
                    string_utils::strings::TEMP_SENSOR_ERROR, machine_status.MSG_LEN);
            return false;
        }

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
        // Check if new PID gains have been requested and update our controller
        double gain;
        if (serial.get_new_kp(&gain))
        {
            pid->set_kp(gain);
        }
        if (serial.get_new_ki(&gain))
        {
            pid->set_ki(gain);
        }
        if (serial.get_new_kd(&gain))
        {
            pid->set_kd(gain);
        }

        if (not pid->compute(machine_status.current_temperature,
                             machine_status.target_temperature,
                             &(machine_status.water_heater_on)))
        {
            machine_status.water_heater_on = false;
            strncpy(machine_status.status_message, string_utils::strings::PID_FAILT,
                    machine_status.MSG_LEN);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////

    BaseTemperatureSensor *water_sensor;
    BaseTemperatureSensor *steam_sensor;
    BasePIDController *pid;
    SerialInterface<Adapter> serial;
    Display<Adapter> display;
    BaseModeDetector *mode_detector;
    Gaggia::ControlStatus machine_status;
    BaseHeater *heater;
};
