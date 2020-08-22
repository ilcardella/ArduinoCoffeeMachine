#include "SerialInterface.h"

SerialInterface::SerialInterface(const uint16_t &baudrate) : time_last_print(0), inputs()
{
    Serial.begin(baudrate);
}

void SerialInterface::read_input()
{
    // Read input data and enable or disable the debug mode
    // If debug mode is enabled, accept temperature input
    // to overwrite sensor readings
    if (Serial.available())
    {
        auto data = Serial.readStringUntil('\n');
        if (data.startsWith("help"))
        {
            print_help();
        }
        else if (data.startsWith("debug on"))
        {
            inputs.debug_mode = true;
            Serial.println("Setting debug mode ON");
        }
        else if (data.startsWith("debug off"))
        {
            inputs.debug_mode = false;
            Serial.println("Setting debug mode OFF");
        }
        else if (data.startsWith("output on"))
        {
            inputs.enable_output = true;
            Serial.println("Setting output ON");
        }
        else if (data.startsWith("output off"))
        {
            inputs.enable_output = false;
            Serial.println("Setting output OFF");
        }
        else if (data.startsWith("temp ") && is_debug_active())
        {
            data.replace("temp ", "");
            inputs.mock_temperature = data.toDouble();
            Serial.println("Setting mock temperature to; " + data);
        }
        else if (data.startsWith("kp "))
        {
            data.replace("kp ", "");
            inputs.kp = data.toDouble();
            Serial.println("Setting PID kp to: " + data);
        }
        else if (data.startsWith("ki "))
        {
            data.replace("ki ", "");
            inputs.ki = data.toDouble();
            Serial.println("Setting PID ki to: " + data);
        }
        else if (data.startsWith("kd "))
        {
            data.replace("kd ", "");
            inputs.kd = data.toDouble();
            Serial.println("Setting PID kd to: " + data);
        }
    }
}

bool SerialInterface::is_output_enabled()
{
    return inputs.enable_output;
}

bool SerialInterface::is_debug_active()
{
    return inputs.debug_mode;
}

double SerialInterface::get_mock_temperature()
{
    return inputs.mock_temperature;
}

bool SerialInterface::get_new_kp(double *kp)
{
    if (inputs.kp > 0.0)
    {
        *kp = inputs.kp;
        inputs.kp = 0.0; // reset input
        return true;
    }
    return false;
}

bool SerialInterface::get_new_ki(double *ki)
{
    if (inputs.ki > 0.0)
    {
        *ki = inputs.ki;
        inputs.ki = 0.0; // reset input
        return true;
    }
    return false;
}

bool SerialInterface::get_new_kd(double *kd)
{
    if (inputs.kd > 0.0)
    {
        *kd = inputs.kd;
        inputs.kd = 0.0; // reset input
        return true;
    }
    return false;
}

void SerialInterface::print_status(Gaggia::ControlStatus *status)
{
    auto now = millis();
    if (is_output_enabled() && now - time_last_print > PRINT_TIMEOUT)
    {
        time_last_print = now;
        // Serial.println("Operation mode: " + String(status->machine_mode));
        // Serial.println("Current temp: " + String(status->current_temperature));
        // Serial.println("Target temp: " + String(status->target_temperature));
        // Serial.println("SSR status: " + String(status->water_heater_on));
        // Serial.println("Message: " + status->status_message);
        String output = String(status->machine_mode) + "," +
                        String(status->current_temperature) + "," +
                        String(status->target_temperature) + "," +
                        String(status->water_heater_on) + "," + status->status_message;
        Serial.println(output);
    }
}

void SerialInterface::print_help()
{
    Serial.println("**** GaggiaPIDController serial interface ****");
    Serial.println("Commands:");
    Serial.println("- help: show this message");
    Serial.println("- debug [on/off]: enable/disable debug mode");
    Serial.println("- output [on/off]: enable/disable serial output");
    Serial.println("- temp [value]: Set the mock temperature. Used only in debug mode");
    Serial.println("- kp [value]: Set the P gain of the PID controller");
    Serial.println("- ki [value]: Set the I gain of the PID controller");
    Serial.println("- kd [value]: Set the D gain of the PID controller");
    Serial.println("**** END ****");
}
