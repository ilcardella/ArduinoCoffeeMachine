#pragma once

#include "lib_coffee_machine/include/lib_coffee_machine/interfaces.h"
#include <Arduino.h>

class ArduinoSerial : public BaseSerialInterface
{
  public:
    ArduinoSerial() = default;

    void begin(const unsigned long &baudrate) override
    {
        Serial.begin(baudrate);
    }

    int available() override
    {
        return Serial.available();
    }

    void read_string_until(const char &terminator, char *data) override
    {
        strcpy(data, Serial.readStringUntil(terminator).c_str());
    }

    size_t println(const char *value) override
    {
        Serial.println(value);
    }
};
