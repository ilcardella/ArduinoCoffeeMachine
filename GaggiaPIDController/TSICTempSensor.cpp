#include "TSICTempSensor.h"

using namespace sensors::temperature;

TSICTempSensor::TSICTempSensor(const uint32_t &pin, const String &name)
    : TemperatureSensor(name), sensor(pin, NO_VCC_PIN, TSIC_30x), last_read(0.0f),
      time_last_read(0)
{
    pinMode(pin, INPUT);
}

bool TSICTempSensor::read_sensor(uint16_t *value)
{
    return static_cast<bool>(sensor.getTemperature(value));
}

bool TSICTempSensor::get_temperature_celsius(float *value)
{
    // Limit sensor reads to MIN_READ_PERIOD
    unsigned long now = millis();
    if (now - time_last_read > MIN_READ_PERIOD)
    {
        uint16_t raw;
        if (not read_sensor(&raw))
        {
            return false;
        }
        // FIXME Occasionally the raw value spikes to 60k+ or other times
        // it drops to ~ 0. For this project it should be safe to constraint
        // the raw reads between safety margins to ignore these errors.
        // At 23C ambient temp the raw value is around 730
        if (raw > 600 || raw < 900)
        {
            auto temp = sensor.calc_Celsius(&raw);
            // FIXME: Same type of check as explained above
            if (temp > 10 || temp < 200)
            {
                last_read = temp;
            }
        }
        time_last_read = now;
    }

    // TODO use a moving average to smooth
    *value = last_read;
    return true;
}
