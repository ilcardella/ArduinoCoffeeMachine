#pragma once

#include <lib_coffee_machine/interfaces.h>

class MockSerial : public BaseSerialInterface
{
  public:
    MockSerial() = default;

    void begin(const unsigned long &baudrate) override
    {
    }

    int available() override
    {
        return input_available ? 1 : 0;
    }

    void read_string_until(const char &terminator, char *data) override
    {
        strcpy(data, input_string);
    }

    size_t println(const char *value) override
    {
        strcpy(output_string, value);
        return strlen(value);
    }

    void set_mock_input(const char *data)
    {
        input_available = true;
        strcpy(input_string, data);
    }

    bool input_available = false;
    char input_string[100];
    char output_string[100];
};
