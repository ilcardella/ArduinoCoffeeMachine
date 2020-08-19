#pragma once

#include "TemperatureSensor.h"
#include <Arduino.h>
#include <TSIC.h>

class TSICTempSensor : public sensors::temperature::TemperatureSensor
{
  public:
    TSICTempSensor(const uint32_t &pin, const String &name);

  protected:
    bool read_sensor(float *value) override;

  private:
    TSIC sensor;
};
