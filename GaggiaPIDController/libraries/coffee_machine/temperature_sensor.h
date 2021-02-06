#pragma once

#include "interfaces.h"
#include "moving_average.h"

template <class Adapter> class TemperatureSensor
{
  public:
    TemperatureSensor(const char *name, BaseSensor *sensor,
                      const unsigned long read_period,
                      const unsigned int &moving_avg_size = 1)
        : name(name), sensor(sensor), m_avg(moving_avg_size),
          time_last_read(Adapter::millis()), healthy(true), read_period(read_period)
    {
    }

    char *get_name()
    {
        return name;
    }

    bool get_temperature_celsius(float *value)
    {
        unsigned long now = Adapter::millis();
        if (now - time_last_read > read_period)
        {
            time_last_read = now;
            float reading;
            if (not sensor->read_sensor(&reading))
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

  private:
    BaseSensor *sensor;
    const char *name;
    unsigned long time_last_read;
    unsigned long read_period;
    MovingAverage<float> m_avg;
    bool healthy;
};
