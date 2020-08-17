#pragma once

#include "TemperatureSensor.h"
#include "max6675.h"
#include <Arduino.h>
#include "libraries/Common.h"

class KTypeThermocouple : public sensors::temperature::TemperatureSensor
{
  public:
    KTypeThermocouple(const String &name, const uint8_t &clk_pin, const uint8_t &do_pin,
                      const uint8_t &cs_pin);

    bool get_temperature_celsius(float *value) override;

  private:
    MAX6675 sensor;
    unsigned long time_last_read;
    static constexpr int READ_PERIOD = 300;
    MovingAverage<float> m_avg;
};
