#include "TSICTempSensor.h"

using namespace sensors::temperature;

TSICTempSensor::TSICTempSensor(const String &name, const uint32_t &pin)
    : TemperatureSensor(name, 300), sensor(pin, NO_VCC_PIN, TSIC_30x)
{
    pinMode(pin, INPUT);
}

bool TSICTempSensor::read_sensor(float *value)
{
    uint16_t raw;
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
