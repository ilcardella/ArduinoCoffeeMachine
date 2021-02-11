#pragma once

#include "common.h"
#include "interfaces.h"

class ModeDetector
{
  public:
    ModeDetector(IOPin *pin) : pin(pin)
    {
        pin->set_mode(IOPin::Modes::IN_PU);
    }

    Machine::Mode get_mode()
    {
        return (pin->is_high()) ? Machine::Mode::WATER_MODE : Machine::Mode::STEAM_MODE;
    }

  private:
    IOPin *pin;
};
