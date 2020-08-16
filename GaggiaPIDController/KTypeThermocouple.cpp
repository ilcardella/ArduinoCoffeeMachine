#include "KTypeThermocouple.h"

KTypeThermocouple::KTypeThermocouple(const String &name, const uint8_t &clk_pin,
                                     const uint8_t &do_pin, const uint8_t &cs_pin)
    : TemperatureSensor(name), sensor(clk_pin, cs_pin, do_pin)
{
}

bool KTypeThermocouple::get_temperature_celsius(float *value)
{
    // Limit sensor reads to READ_PERIOD
    unsigned long now = millis();
    if (now - time_last_read > READ_PERIOD)
    {
        float reading = sensor.readCelsius();
        if (reading == NAN)
        {
            return false;
        }

        last_read_value = reading;
        time_last_read = now;
    }

    // TODO use a moving average to smooth
    *value = last_read_value;
    return true;
}
