#pragma once

#include <coffee_machine/BaseTypes.h>

class MockHeater : public BaseHeater
{
  public:
    MockHeater() = default;

    bool set(const bool &on) override
    {
        return healthy;
    }

    void reset()
    {
        healthy = true;
    }

    bool healthy = true;
};
