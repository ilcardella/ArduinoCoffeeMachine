#pragma once

#include <Arduino.h>
#include <Wire.h>

class ArduinoAdapter
{
  public:
    inline static void pinMode(uint8_t pin, uint8_t mode)
    {
        ::pinMode(pin, mode);
    }

    inline static int digitalRead(uint8_t pin)
    {
        return ::digitalRead(pin);
    }

    inline static void digitalWrite(uint8_t pin, uint8_t val)
    {
        ::digitalWrite(pin, val);
    }

    inline static void delay(unsigned long ms)
    {
        ::delay(ms);
    }

    inline static unsigned long millis()
    {
        return ::millis();
    }

    inline static void SerialBegin(const unsigned long &baudrate)
    {
        Serial.begin(baudrate);
    }

    inline static int SerialAvailable()
    {
        return Serial.available();
    }

    inline static void SerialReadStringUntil(const char &terminator, char *data)
    {
        strcpy(data, Serial.readStringUntil(terminator).c_str());
    }

    inline static size_t SerialPrintln(const char *value)
    {
        return Serial.println(value);
    }

    inline static void WireBegin()
    {
        Wire.begin();
    }

    inline static void WireSetClock(const uint32_t &clock)
    {
        Wire.setClock(clock);
    }
};
