#pragma once

#include "TemperatureSensor.h"
#include <Arduino.h>
#include <TSIC.h>

class TSICTempSensor : public sensors::temperature::TemperatureSensor
{
  public:
    TSICTempSensor(const String &name, const uint32_t &pin);

  protected:
    bool read_sensor(float *value) override;

  private:
    TSIC sensor;
};
