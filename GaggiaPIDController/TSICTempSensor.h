#pragma once

#include "TemperatureSensor.h"
#include <Arduino.h>
#include <TSIC.h>

class TSICTempSensor : public sensors::temperature::TemperatureSensor
{
  public:
    TSICTempSensor(const uint32_t &pin, const String &name);

    bool get_temperature_celsius(float *value) override;

  private:
    bool read_sensor(uint16_t *value);

    TSIC sensor;
    float last_read;
    unsigned long time_last_read;
    static constexpr int MIN_READ_PERIOD = 300;
};
