#include "KTypeThermocouple.h"

KTypeThermocouple::KTypeThermocouple(const String &name, const uint8_t &clk_pin,
                                     const uint8_t &do_pin, const uint8_t &cs_pin)
    : TemperatureSensor(name, 300), sensor(clk_pin, cs_pin, do_pin)
{
}

bool KTypeThermocouple::read_sensor(float *value)
{
    float reading = sensor.readCelsius();
    if (reading == NAN)
    {
        return false;
    }
    *value = reading;
    return true;
}
