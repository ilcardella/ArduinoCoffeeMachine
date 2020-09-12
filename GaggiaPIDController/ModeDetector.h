#pragma once

#include "Common.h"

template <class Adapter> class ModeDetector
{
  public:
    ModeDetector(const uint32_t &pin) : steam_switch_pin(pin)
    {
        Adapter::pinMode(pin, INPUT_PULLUP);
    }

    Gaggia::Mode get_mode()
    {
        return (Adapter::digitalRead(steam_switch_pin) == HIGH)
                   ? Gaggia::Mode::WATER_MODE
                   : Gaggia::Mode::STEAM_MODE;
    }

  private:
    uint32_t steam_switch_pin;
};
