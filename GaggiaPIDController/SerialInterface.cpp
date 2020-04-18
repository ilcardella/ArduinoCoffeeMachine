#include "SerialInterface.h"

SerialInterface::SerialInterface(const uint16_t &baudrate)
    : time_last_print(0), debug_mode(false), mock_temperature(0.0), enable_output(false)
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
        auto input = Serial.readStringUntil('\n');
        if (input.startsWith("debug on"))
        {
            debug_mode = true;
        }
        else if (input.startsWith("debug off"))
        {
            debug_mode = false;
        }
        else if (input.startsWith("output on"))
        {
            enable_output = true;
        }
        else if(input.startsWith("output off"))
        {
            enable_output = false;
        }
        else if (is_debug_active())
        {
            mock_temperature = input.toDouble();
        }
    }
}

void SerialInterface::print_status(Gaggia::ControlStatus *status)
{
    auto now = millis();
    if (is_output_enabled() && now - time_last_print > PRINT_TIMEOUT)
    {
        time_last_print = now;
        Serial.println("Operation mode: " + String(status->machine_mode));
        Serial.println("Current temp: " + String(status->current_temperature));
        Serial.println("Target temp: " + String(status->target_temperature));
        Serial.println("SSR status: " + String(status->water_heater_on));
        Serial.println("Message: " + status->status_message);
    }
}

bool SerialInterface::is_output_enabled()
{
    return enable_output;
}

bool SerialInterface::is_debug_active()
{
    return debug_mode;
}

double SerialInterface::get_mock_temperature()
{
    return mock_temperature;
}
