#pragma once

#include "Sensors.h"
#include "coffee_machine/BaseTypes.h"
#include "coffee_machine/Configuration.h"
#include "coffee_machine/TemperatureSensor.h"

class SensorFactory
{
  public:
    template <class Adapter, SensorTypes type>
    static BaseTemperatureSensor<Adapter> *make_temperature_sensor(
        const typename Adapter::String &name, const unsigned char &pin)
    {
        using namespace sensors;
        switch (type)
        {
        case SensorTypes::TSIC:
            return new TemperatureSensor<Adapter, TSICTempSensor>(name, pin, 300, 10);
            break;
        case SensorTypes::KTYPE_SPI:
            return new TemperatureSensor<Adapter, KTypeThermocouple>(name, pin, 300, 10,
                                                                     -10.0f);
            break;
        default:
            // Ideally we would raise an exception here
            return nullptr;
            break;
        }
    }
};
