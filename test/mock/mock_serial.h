#pragma once

#include <coffee_machine/interfaces.h>

class MockSerial : public BaseSerialInterface
{
  public:
    MockSerial() = default;

    void begin(const unsigned long &baudrate) override
    {
    }

    int available() override
    {
        return 1;
    }

    void read_string_until(const char &terminator, char *data) override
    {
        read_input_called = true;
        strncpy(data, input_string, 100);
    }

    size_t println(const char *value) override
    {
        strncpy(output_string, value, 100);
        return strlen(value);
    }

    void reset()
    {
        read_input_called = false;
    }

    char input_string[100];
    char output_string[100];
    bool read_input_called = false;
};
