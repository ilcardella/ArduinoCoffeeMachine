#pragma once

#include "coffee_machine/configuration.h"
#include "coffee_machine/interfaces.h"

#include <max6675.h>

class KTypeThermocouple : public BaseSensor
{
  public:
    KTypeThermocouple(const unsigned char &pin)
        : sensor(Configuration::SPI_CLK_PIN, pin, Configuration::SPI_DO_PIN)
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
