#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(const uint32_t &pin, const String &name)
    : sensor(pin, NO_VCC_PIN, TSIC_30x), name(name), last_read(0.0f), time_last_read(0)
{
}

String TemperatureSensor::get_name()
{
    return name;
}

bool TemperatureSensor::read_sensor(uint16_t *value)
{
    return static_cast<bool>(sensor.getTemperature(value));
}

bool TemperatureSensor::get_temperature_celsius(float *value)
{
    // Limit sensor reads to 1 every MIN_READ_PERIOD
    unsigned long now = millis();
    if (now - time_last_read < MIN_READ_PERIOD)
    {
        return last_read;
    }

    uint16_t raw;
    if (read_sensor(&raw))
    {
        last_read = sensor.calc_Celsius(&raw);
        time_last_read = now;
        *value = last_read;
        return true;
    }
    return false;
}
