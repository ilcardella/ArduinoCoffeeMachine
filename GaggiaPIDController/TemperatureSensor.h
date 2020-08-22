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
    TemperatureSensor(const String &name, const uint32_t read_period,
                      const uint32_t &moving_avg_size = 1)
        : name(name), m_avg(moving_avg_size), time_last_read(millis()), healthy(true),
          read_period(read_period)
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
    bool get_temperature_celsius(float *value)
    {
        unsigned long now = millis();
        if (now - time_last_read > read_period)
        {
            time_last_read = now;
            float reading;
            if (not read_sensor(&reading))
            {
                healthy = false;
                return false;
            }

            healthy = true;
            m_avg.add(reading);
        }

        *value = m_avg.get();
        return healthy;
    }

  protected:
    virtual bool read_sensor(float *value) = 0;

    String name;
    unsigned long time_last_read;
    uint32_t read_period;
    MovingAverage<float> m_avg;
    bool healthy;
};
} // namespace temperature
} // namespace sensors
