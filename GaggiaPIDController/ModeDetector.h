#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

class ModeDetector
{
  public:
    ModeDetector(const uint32_t &pin) : steam_switch_pin(pin)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    Gaggia::MODE get_mode()
    {
        return (digitalRead(steam_switch_pin) == HIGH) ? Gaggia::WATER_MODE
                                                       : Gaggia::STEAM_MODE;
    }

  private:
    uint32_t steam_switch_pin;
};
