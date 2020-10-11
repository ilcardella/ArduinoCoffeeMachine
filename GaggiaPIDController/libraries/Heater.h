#pragma once

#include "BaseTypes.h"

template <class Adapter> class Heater : public BaseHeater
{
  public:
    Heater(const unsigned char &pin) : pin(pin)
    {
        Adapter::pinMode(pin, OUTPUT);
    }

    bool set(const bool &on) override
    {
        Adapter::digitalWrite(pin, on ? HIGH : LOW);
    }

  private:
    unsigned char pin;
};
