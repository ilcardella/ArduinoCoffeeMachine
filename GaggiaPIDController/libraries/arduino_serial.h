#pragma once

#include "coffee_machine/interfaces.h"
#include <Arduino.h>

template <class Adapter> class ArduinoSerial : public BaseSerialInterface
{
  public:
    ArduinoSerial(const unsigned short &baudrate)
    {
        begin(baudrate);
    }

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
