#pragma once

#include <coffee_machine/BaseTypes.h>
#include <coffee_machine/Common.h>

template <class Adapter> class MockDisplay : public BaseDisplay<Adapter>
{
  public:
    MockDisplay() = default;
    bool update(const Gaggia::ControlStatus<Adapter> &status) override
    {
        return healthy;
    }

    void reset()
    {
        healthy = true;
    }

    bool healthy = true;
};
