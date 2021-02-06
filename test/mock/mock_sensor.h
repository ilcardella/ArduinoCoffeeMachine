#pragma once

#include <coffee_machine/interfaces.h>

class MockSensor : public BaseSensor
{
  public:
    MockSensor() = default;

    bool read_sensor(float *value) override
    {
        *value = temp_c;
        return healthy;
    }

    float temp_c = 0.0f;
    bool healthy = true;
};
