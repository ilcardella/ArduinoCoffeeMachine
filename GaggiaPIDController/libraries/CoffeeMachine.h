#pragma once

#include "BaseTypes.h"
#include "Common.h"
#include "Configuration.h"

template <class Adapter> class CoffeeMachine
{
  public:
    CoffeeMachine(BasePIDController *pid, BaseSerialInterface<Adapter> *serial,
                  BaseModeDetector *mode_detector, BaseDisplay<Adapter> *display,
                  BaseHeater *heater, BaseTemperatureSensor<Adapter> *water_sensor,
                  BaseTemperatureSensor<Adapter> *steam_sensor)
        : pid(pid), serial(serial), mode_detector(mode_detector), display(display),
          heater(heater), water_sensor(water_sensor), steam_sensor(steam_sensor),
          machine_status()
    {
        // Mark machine start time
        machine_status.time_since_start = Adapter::millis();
    }

    bool spin()
    {
        serial->read_input();

        if (update_machine_status(machine_status))
        {
            update_pid(machine_status);
        }

        heater->set(machine_status.water_heater_on);

        display->update(machine_status);

        serial->print_status(machine_status);
        return true;
    }

  private:
    bool update_machine_status(Gaggia::ControlStatus<Adapter> &status)
    {
        unsigned long now = Adapter::millis();

        // Set SSR off by default and let the PID decide whether to turn it on
        status.water_heater_on = false;
        // Read operation mode
        status.machine_mode = mode_detector->get_mode();

        // Reset steam mode timeout counter when not in steam mode
        if (status.machine_mode != Gaggia::Mode::STEAM_MODE)
        {
            status.time_since_steam_mode = now;
        }

        // Set target temperature based on machine mode
        status.target_temperature = (status.machine_mode == Gaggia::Mode::WATER_MODE)
                                        ? Configuration::TARGET_WATER_TEMP
                                        : Configuration::TARGET_STEAM_TEMP;

        // When debug mode is enabled do not read sensors
        if (serial->is_debug_active())
        {
            status.current_temperature = serial->get_mock_temperature();
            status.status_message = "Debug mode";
            return true;
        }

        // Select correct sensor for current operation mode
        BaseTemperatureSensor<Adapter> *sensor =
            (status.machine_mode == Gaggia::Mode::WATER_MODE) ? water_sensor
                                                              : steam_sensor;

        // Get the current temp from the temperature sensor
        float sensor_value;
        if (not sensor || not sensor->get_temperature_celsius(&sensor_value))
        {
            status.status_message =
                "Unable to read temperature from sensor: " + sensor->get_name();
            return false;
        }

        status.current_temperature = static_cast<double>(sensor_value);

        // Use a tolerance of +/- 1Deg for the message
        double diff = status.target_temperature - status.current_temperature;
        if (diff < -1.0)
        {
            status.status_message = "Cooling...";
        }
        else if (diff > 1.0)
        {
            status.status_message = "Heating...";
        }
        else
        {
            status.status_message = "Ready";
        }

        // If the machine has been on for more than the safety limit, then report a
        // problem so the heater will be turned off
        if (Configuration::SAFETY_TIMEOUT > 0 &&
            (now - status.time_since_start) > Configuration::SAFETY_TIMEOUT)
        {
            status.status_message = "Safety timeout expired";
            return false;
        }
        // Check steam mode timeout to avoid keeping the machine at high temps for long
        if (Configuration::STEAM_TIMEOUT > 0 &&
            (now - status.time_since_steam_mode) > Configuration::STEAM_TIMEOUT)
        {
            status.status_message = "Steam mode timeout expired";
            return false;
        }

        return true;
    }

    void update_pid(Gaggia::ControlStatus<Adapter> &status)
    {
        // Check if new PID gains have been requested and update our controller
        double gain;
        if (serial->get_new_kp(&gain))
        {
            pid->set_kp(gain);
        }
        if (serial->get_new_ki(&gain))
        {
            pid->set_ki(gain);
        }
        if (serial->get_new_kd(&gain))
        {
            pid->set_kd(gain);
        }

        pid->compute(status.current_temperature, status.target_temperature,
                     &(status.water_heater_on));
    }

    /////////////////////////////////////////////////////////////////////////////////////

    BaseTemperatureSensor<Adapter> *water_sensor;
    BaseTemperatureSensor<Adapter> *steam_sensor;
    BasePIDController *pid;
    BaseSerialInterface<Adapter> *serial;
    BaseDisplay<Adapter> *display;
    BaseModeDetector *mode_detector;
    Gaggia::ControlStatus<Adapter> machine_status;
    BaseHeater *heater;
};
