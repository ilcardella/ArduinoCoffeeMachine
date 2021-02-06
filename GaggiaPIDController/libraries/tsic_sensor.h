#pragma once

#include "coffee_machine/interfaces.h"

// #include <Arduino.h>
#include <TSIC.h>

class TSICTempSensor : public BaseSensor
{
  public:
    TSICTempSensor(const unsigned char &pin) : sensor(pin, NO_VCC_PIN, TSIC_30x)
    {
        pinMode(pin, INPUT);
    }

    bool read_sensor(float *value) override
    {
        unsigned short raw;
        if (not sensor.getTemperature(&raw))
        {
            return false;
        }
        // FIXME Occasionally the raw value spikes to 60k+ or other times
        // it drops to ~ 0. For this project it should be safe to constraint
        // the raw reads between safety margins to ignore these errors.
        // At 23C ambient temp the raw value is around 730
        if (raw < 600 || raw > 900)
        {
            return false;
        }

        auto temp = sensor.calc_Celsius(&raw);
        // FIXME: Same type of check as explained above
        if (temp < 10 || temp > 200)
        {
            return false;
        }

        *value = temp;
        return true;
    }

  private:
    TSIC sensor;
};