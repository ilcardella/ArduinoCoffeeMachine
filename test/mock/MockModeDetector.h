#pragma once

#include <coffee_machine/BaseTypes.h>
#include <coffee_machine/Common.h>

class MockModeDetector : public BaseModeDetector
{
  public:
    MockModeDetector() = default;

    Gaggia::Mode get_mode() override
    {
        return mode;
    }

    void reset()
    {
        mode = Gaggia::Mode::WATER_MODE;
    }

    Gaggia::Mode mode = Gaggia::Mode::WATER_MODE;
};
