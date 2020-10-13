#pragma once

#include <chrono>
#include <string>
#include <unistd.h>

class CppAdapter
{
  public:
    using String = std::string;
    using StringSumHelper = std::string;

    static void pinMode(uint8_t pin, uint8_t mode)
    {
        ;
    }

    static int digitalRead(uint8_t pin)
    {
        return 0;
    }

    static void digitalWrite(uint8_t pin, uint8_t val)
    {
        ;
    }

    static void delay(unsigned long ms)
    {
        sleep(ms / 1000);
    }

    static unsigned long millis()
    {
        // using namespace std::chrono;
        // return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
        //     .count();
        return millis_ret;
    }

    static void SerialBegin(const unsigned long &baudrate)
    {
        ;
    }

    static int SerialAvailable()
    {
        return 1;
    }

    static String SerialReadStringUntil(const char &terminator)
    {
        return "";
    }

    static size_t SerialPrintln(const char *value)
    {
        return 0;
    }

    static size_t SerialPrintln(const String &value)
    {
        return 0;
    }

    static size_t SerialPrintln(StringSumHelper &value)
    {
        return 0;
    }

    inline static unsigned long millis_ret = 0;
};
