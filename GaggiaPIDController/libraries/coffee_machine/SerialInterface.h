#pragma once

#include "BaseTypes.h"
#include "Common.h"

template <class Adapter> class SerialInterface : public BaseSerialInterface
{
  public:
    SerialInterface(const unsigned short &baudrate) : time_last_print(0), inputs()
    {
        Adapter::SerialBegin(baudrate);
    }

    void read_input() override
    {
        // Read input data and enable or disable the debug mode
        // If debug mode is enabled, accept temperature input
        // to overwrite sensor readings
        if (Adapter::SerialAvailable())
        {
            char data[25];
            Adapter::SerialReadStringUntil('\n', data);
            if (string_utils::start_with(data, "help"))
            {
                print_help();
            }
            else if (string_utils::start_with(data, "debug on"))
            {
                inputs.debug_mode = true;
                Adapter::SerialPrintln("Setting debug mode ON");
            }
            else if (string_utils::start_with(data, "debug off"))
            {
                inputs.debug_mode = false;
                Adapter::SerialPrintln("Setting debug mode OFF");
            }
            else if (string_utils::start_with(data, "output on"))
            {
                inputs.enable_output = true;
                Adapter::SerialPrintln("Setting output ON");
            }
            else if (string_utils::start_with(data, "output off"))
            {
                inputs.enable_output = false;
                Adapter::SerialPrintln("Setting output OFF");
            }
            else if (string_utils::start_with(data, "temp ") && is_debug_active())
            {
                inputs.mock_temperature = string_utils::to_number<double>(data, 5);
                // Adapter::SerialPrintln("Setting mock temperature to; " + data);
                Adapter::SerialPrintln("Setting mock temperature");
            }
            else if (string_utils::start_with(data, "kp "))
            {
                inputs.kp = string_utils::to_number<double>(data, 3);
                // Adapter::SerialPrintln("Setting PID kp to: " + data);
                Adapter::SerialPrintln("Setting PID kp");
            }
            else if (string_utils::start_with(data, "ki "))
            {
                inputs.ki = string_utils::to_number<double>(data, 3);
                // Adapter::SerialPrintln("Setting PID ki to: " + data);
                Adapter::SerialPrintln("Setting PID ki");
            }
            else if (string_utils::start_with(data, "kd "))
            {
                inputs.kd = string_utils::to_number<double>(data, 3);
                // Adapter::SerialPrintln("Setting PID kd to: " + data);
                Adapter::SerialPrintln("Setting PID kd");
            }
        }
    }

    void print_status(const Gaggia::ControlStatus &status) override
    {
        auto now = Adapter::millis();
        if (is_output_enabled() && now - time_last_print > PRINT_TIMEOUT)
        {
            char output[100];
            char curr_temp_buffer[6];
            char target_temp_buffer[6];

            time_last_print = now;

            Adapter::dtostrf(status.current_temperature, 4, 1, curr_temp_buffer);
            Adapter::dtostrf(status.target_temperature, 4, 1, target_temp_buffer);

            snprintf(output, 100, "%d,%s,%s,%d,%s", static_cast<int>(status.machine_mode),
                     curr_temp_buffer, target_temp_buffer, status.water_heater_on,
                     status.status_message);
            Adapter::SerialPrintln(output);
        }
    }

    bool is_debug_active() override
    {
        return inputs.debug_mode;
    }

    double get_mock_temperature() override
    {
        return inputs.mock_temperature;
    }

    bool get_new_kp(double *kp) override
    {
        if (inputs.kp > 0.0)
        {
            *kp = inputs.kp;
            inputs.kp = 0.0; // reset input
            return true;
        }
        return false;
    }

    bool get_new_ki(double *ki) override
    {
        if (inputs.ki > 0.0)
        {
            *ki = inputs.ki;
            inputs.ki = 0.0; // reset input
            return true;
        }
        return false;
    }

    bool get_new_kd(double *kd) override
    {
        if (inputs.kd > 0.0)
        {
            *kd = inputs.kd;
            inputs.kd = 0.0; // reset input
            return true;
        }
        return false;
    }

  private:
    void print_help()
    {
        Adapter::SerialPrintln("**** GaggiaPIDController serial interface ****");
        Adapter::SerialPrintln("Commands:");
        Adapter::SerialPrintln("- help: show this message");
        Adapter::SerialPrintln("- debug [on/off]: enable/disable debug mode");
        Adapter::SerialPrintln("- output [on/off]: enable/disable serial output");
        Adapter::SerialPrintln(
            "- temp [value]: Set the mock temperature. Used only in debug mode");
        Adapter::SerialPrintln("- kp [value]: Set the P gain of the PID controller");
        Adapter::SerialPrintln("- ki [value]: Set the I gain of the PID controller");
        Adapter::SerialPrintln("- kd [value]: Set the D gain of the PID controller");
        Adapter::SerialPrintln("**** END ****");
    }

    bool is_output_enabled()
    {
        return inputs.enable_output;
    }

    static constexpr unsigned short PRINT_TIMEOUT = 200;
    unsigned long time_last_print;

    struct SerialInput
    {
        bool debug_mode = false;
        bool enable_output = false;
        double mock_temperature = 0.0;
        double kp = 0.0;
        double ki = 0.0;
        double kd = 0.0;
    };

    SerialInput inputs;
};
