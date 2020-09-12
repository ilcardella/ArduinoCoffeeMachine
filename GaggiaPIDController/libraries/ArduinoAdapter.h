#pragma once

#include <Arduino.h>

class ArduinoAdapter
{
  public:
    using String = String;
    using StringSumHelper = StringSumHelper;

    static void pinMode(uint8_t pin, uint8_t mode)
    {
        pinMode(pin, mode);
    }

    static int digitalRead(uint8_t pin)
    {
        return digitalRead(pin);
    }

    static void digitalWrite(uint8_t pin, uint8_t val)
    {
        digitalWrite(pin, val);
    }

    static void delay(unsigned long ms)
    {
        delay(ms);
    }

    static unsigned long millis()
    {
        return millis();
    }

    static void SerialBegin(const unsigned long &baudrate)
    {
        Serial.begin(baudrate);
    }

    static int SerialAvailable()
    {
        return Serial.available();
    }

    static String SerialReadStringUntil(const char &terminator)
    {
        return Serial.readStringUntil(terminator);
    }

    static size_t SerialPrintln(const char *value)
    {
        return Serial.println(value);
    }

    static size_t SerialPrintln(const String &value)
    {
        return Serial.println(value);
    }

    static size_t SerialPrintln(StringSumHelper &value)
    {
        return Serial.println(value);
    }
};
