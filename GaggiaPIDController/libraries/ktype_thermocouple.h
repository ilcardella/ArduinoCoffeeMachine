#pragma once

#include "coffee_machine/interfaces.h"

#include <max6675.h>

class KTypeThermocouple : public BaseSensor
{
  public:
    KTypeThermocouple(const unsigned char &sensor_pin, const unsigned char &clk_pin,
                      const unsigned char &do_pin)
        : sensor(clk_pin, sensor_pin, do_pin)
    {
    }

    bool read_sensor(float *value) override
    {
        float reading = sensor.readCelsius();
        if (reading == NAN)
        {
            return false;
        }
        *value = reading;
        return true;
    }

  private:
    MAX6675 sensor;
};
