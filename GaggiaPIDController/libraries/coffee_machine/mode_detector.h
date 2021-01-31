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

    Gaggia::Mode get_mode()
    {
        return (pin->is_high()) ? Gaggia::Mode::WATER_MODE : Gaggia::Mode::STEAM_MODE;
    }

  private:
    IOPin *pin;
};
