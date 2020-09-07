#pragma once

#include "Common.h"
#include <Arduino.h>

class BaseTemperatureSensor
{
  public:
    /** Return the name identifier of the sensor.
     */
    virtual String get_name() = 0;

    /* Read the sensor and store the current temperature in
     * celsius degrees into 'value'.
     * Return 'true' if the operation succeeds, 'false' otherwise */
    virtual bool get_temperature_celsius(float *value) = 0;
};

template <class SensorType> class TemperatureSensor : public BaseTemperatureSensor
{
  public:
    TemperatureSensor(const String &name, const uint8_t &pin, const uint32_t read_period,
                      const uint32_t &moving_avg_size = 1, const float &temp_offset = 0)
        : name(name), m_avg(moving_avg_size), time_last_read(millis()), healthy(true),
          read_period(read_period), temp_offset(temp_offset), sensor(pin)
    {
    }

    String get_name()
    {
        return name;
    }

    bool get_temperature_celsius(float *value)
    {
        unsigned long now = millis();
        if (now - time_last_read > read_period)
        {
            time_last_read = now;
            float reading;
            if (not sensor.read_sensor(&reading))
            {
                healthy = false;
                return false;
            }
            reading += temp_offset;

            healthy = true;
            m_avg.add(reading);
        }
        *value = m_avg.get();
        return healthy;
    }

  private:
    SensorType sensor;
    String name;
    unsigned long time_last_read;
    uint32_t read_period;
    MovingAverage<float> m_avg;
    bool healthy;
    float temp_offset;
};
