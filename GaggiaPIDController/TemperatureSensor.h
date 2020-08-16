#pragma once

#include <Arduino.h>

namespace sensors
{
namespace temperature
{
enum class type
{
    TSIC = 0, // TSic sensor family
    KTYPE_SPI = 1 // K-type thermocouple with SPI interface
};

class TemperatureSensor
{
  public:
    TemperatureSensor(const String &name) : name(name)
    {
    }

    /** Return the name identifier of the sensor.
     */
    virtual String get_name()
    {
        return name;
    }

    /* Read the sensor and store the current temperature in
     * celsius degrees into 'value'.
     * Return 'true' if the operation succeeds, 'false' otherwise */
    virtual bool get_temperature_celsius(float *value);

  private:
    String name;
};
} // namespace temperature
} // namespace sensors
