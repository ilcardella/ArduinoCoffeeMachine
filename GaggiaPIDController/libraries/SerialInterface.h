#pragma once

#include "coffee_machine/BaseTypes.h"
#include "coffee_machine/Common.h"

template <class Adapter> class SerialInterface : public BaseSerialInterface<Adapter>
{
  public:
    SerialInterface(const unsigned short &baudrate) : time_last_print(0), inputs()
    {
        Adapter::SerialBegin(baudrate);
    }

    void read_input() override
    {
        using string = typename Adapter::String;

        // Read input data and enable or disable the debug mode
        // If debug mode is enabled, accept temperature input
        // to overwrite sensor readings
        if (Adapter::SerialAvailable())
        {
            string data = Adapter::SerialReadStringUntil('\n');
            if (data.startsWith("help"))
            {
                print_help();
            }
            else if (data.startsWith("debug on"))
            {
                inputs.debug_mode = true;
                Adapter::SerialPrintln("Setting debug mode ON");
            }
            else if (data.startsWith("debug off"))
            {
                inputs.debug_mode = false;
                Adapter::SerialPrintln("Setting debug mode OFF");
            }
            else if (data.startsWith("output on"))
            {
                inputs.enable_output = true;
                Adapter::SerialPrintln("Setting output ON");
            }
            else if (data.startsWith("output off"))
            {
                inputs.enable_output = false;
                Adapter::SerialPrintln("Setting output OFF");
            }
            else if (data.startsWith("temp ") && is_debug_active())
            {
                data.replace("temp ", "");
                inputs.mock_temperature = data.toDouble();
                Adapter::SerialPrintln("Setting mock temperature to; " + data);
            }
            else if (data.startsWith("kp "))
            {
                data.replace("kp ", "");
                inputs.kp = data.toDouble();
                Adapter::SerialPrintln("Setting PID kp to: " + data);
            }
            else if (data.startsWith("ki "))
            {
                data.replace("ki ", "");
                inputs.ki = data.toDouble();
                Adapter::SerialPrintln("Setting PID ki to: " + data);
            }
            else if (data.startsWith("kd "))
            {
                data.replace("kd ", "");
                inputs.kd = data.toDouble();
                Adapter::SerialPrintln("Setting PID kd to: " + data);
            }
        }
    }

    void print_status(const Gaggia::ControlStatus<Adapter> &status) override
    {
        using string = typename Adapter::String;
        auto now = Adapter::millis();
        if (is_output_enabled() && now - time_last_print > PRINT_TIMEOUT)
        {
            time_last_print = now;
            string output = string(static_cast<int>(status.machine_mode)) + "," +
                            string(status.current_temperature) + "," +
                            string(status.target_temperature) + "," +
                            string(status.water_heater_on) + "," + status.status_message;
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
        bool enable_output = true;
        double mock_temperature = 0.0;
        double kp = 0.0;
        double ki = 0.0;
        double kd = 0.0;
    };

    SerialInput inputs;
};
