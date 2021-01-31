#pragma once

#include "interfaces.h"

class Heater
{
  public:
    Heater(IOPin *heater_pin) : pin(heater_pin)
    {
        pin->set_mode(IOPin::Modes::OUT);
    }

    bool set(const bool &on)
    {
        if (on)
        {
            pin->digital_write_high();
        }
        else
        {
            pin->digital_write_low();
        }
    }

  private:
    IOPin *pin;
};
