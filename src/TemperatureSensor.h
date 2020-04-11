#pragma once

#include <Arduino.h>
#include <TSIC.h>

class TemperatureSensor
{
  public:
    TemperatureSensor(const uint32_t &pin, const String &name);

    String get_name();

    /* Read the sensor and store the current temperature in
     * celsius degrees tnto 'value'.
     * Return 'true' if the operation succeed, 'false' otherwise */
    bool get_temperature_celsius(float *value);

  private:
    bool read_sensor(uint16_t *value);

    TSIC sensor;
    String name;
    float last_read;
    unsigned long time_last_read;
    static constexpr int MIN_READ_PERIOD = 100;
};
