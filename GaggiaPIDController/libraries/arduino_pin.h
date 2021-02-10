#pragma once

#include "lib_coffee_machine/src/lib_coffee_machine/interfaces.h"
#include <Arduino.h>

class ArduinoPin : public IOPin
{
  public:
    ArduinoPin(const unsigned char &pin) : pin(pin)
    {
    }

    void set_mode(const IOPin::Modes &mode)
    {
        uint8_t pin_mode(0);
        switch (mode)
        {
        case IOPin::Modes::IN_PU:
            pin_mode = INPUT_PULLUP;
            break;
        default:
            break;
        }
        pinMode(pin, pin_mode);
    }

    bool is_high() override
    {
        return digitalRead(pin) == HIGH;
    }

    bool is_low() override
    {
        return not is_high();
    }

    void digital_write_high() override
    {
        digitalWrite(pin, HIGH);
    }

    void digital_write_low() override
    {
        digitalWrite(pin, LOW);
    }

  private:
    unsigned char pin;
};
