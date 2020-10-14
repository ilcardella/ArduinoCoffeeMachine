#pragma once

#include <coffee_machine/BaseTypes.h>

template <class Adapter> class MockTempSensor : public BaseTemperatureSensor<Adapter>
{
  public:
    MockTempSensor(const typename Adapter::String &name) : name(name)
    {
    }

    typename Adapter::String get_name() override
    {
        return name;
    }

    bool get_temperature_celsius(float *value) override
    {
        *value = temp_c;
        return healthy;
    }

    void reset()
    {
        temp_c = 0.0f;
        healthy = true;
    }

    typename Adapter::String name;
    float temp_c = 0.0f;
    bool healthy = true;
};
