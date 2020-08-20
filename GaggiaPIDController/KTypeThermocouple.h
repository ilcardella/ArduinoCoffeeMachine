#pragma once

#include "TemperatureSensor.h"
#include "max6675.h"
#include <Arduino.h>

class KTypeThermocouple : public sensors::temperature::TemperatureSensor
{
  public:
    KTypeThermocouple(const String &name, const uint8_t &clk_pin, const uint8_t &do_pin,
                      const uint8_t &cs_pin);

  protected:
    bool read_sensor(float *value) override;

  private:
    MAX6675 sensor;
};
