#pragma once

#include <string>
#include <unistd.h>

class TestAdapter
{
  public:
    inline static void pinMode(uint8_t pin, uint8_t mode)
    {
        ;
    }

    inline static int digitalRead(uint8_t pin)
    {
        return 0;
    }

    inline static void digitalWrite(uint8_t pin, uint8_t val)
    {
        ;
    }

    inline static void delay(unsigned long ms)
    {
        sleep(ms / 1000);
    }

    inline static unsigned long millis()
    {
        // using namespace std::chrono;
        // return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
        //     .count();
        return millis_ret;
    }

    inline static void SerialBegin(const unsigned long &baudrate)
    {
        ;
    }

    inline static int SerialAvailable()
    {
        return 1;
    }

    inline static void SerialReadStringUntil(const char &terminator, char *data)
    {
        ;
    }

    inline static size_t SerialPrintln(const char *value)
    {
        strncpy(message_sent, value, 100);
        return 0;
    }

    inline static void WireBegin()
    {
        ;
    }

    inline static void WireSetClock(const uint32_t &clock)
    {
        ;
    }

    inline static char *dtostrf(double val, signed char width, unsigned char prec,
                                char *sout)
    {
        char fmt[20];
        sprintf(fmt, "%%%d.%df", width, prec);
        sprintf(sout, fmt, val);
        return sout;
    }

    inline static unsigned long millis_ret = 0;
    inline static char message_sent[100];
};
