#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

namespace sensors
{
namespace temperature
{
enum class type
{
    TSIC = 0,     // TSic sensor family
    KTYPE_SPI = 1 // K-type thermocouple with SPI interface
};

class TemperatureSensor
{
  public:
    TemperatureSensor(const String &name)
        : name(name), m_avg(10), time_last_read(millis())
    {
    }

    /** Return the name identifier of the sensor.
     */
    String get_name()
    {
        return name;
    }

    /* Read the sensor and store the current temperature in
     * celsius degrees into 'value'.
     * Return 'true' if the operation succeeds, 'false' otherwise */
    virtual bool get_temperature_celsius(float *value)
    {
        unsigned long now = millis();
        if (now - time_last_read > READ_PERIOD)
        {
            time_last_read = now;
            float reading;
            if (not read_sensor(&reading))
            {
                return false;
            }

            m_avg.add(reading);
        }

        *value = m_avg.get();
        return true;
    }

  protected:
    virtual bool read_sensor(float *value) = 0;

    String name;
    unsigned long time_last_read;
    static constexpr int READ_PERIOD = 300;
    MovingAverage<float> m_avg;
};
} // namespace temperature
} // namespace sensors
