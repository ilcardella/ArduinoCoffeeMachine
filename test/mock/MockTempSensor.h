#pragma once

#include <coffee_machine/BaseTypes.h>

class MockTempSensor : public BaseTemperatureSensor
{
  public:
    MockTempSensor(std::string name) : name(name)
    {
    }

    char *get_name() override
    {
        return const_cast<char *>(name.c_str());
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

    std::string name;
    float temp_c = 0.0f;
    bool healthy = true;
};
