#pragma once

template <class Adapter> class Heater
{
  public:
    Heater(const unsigned char &pin) : pin(pin)
    {
        Adapter::pinMode(pin, OUTPUT);
    }

    bool set(const bool &on)
    {
        Adapter::digitalWrite(pin, on ? HIGH : LOW);
    }

  private:
    unsigned char pin;
};
