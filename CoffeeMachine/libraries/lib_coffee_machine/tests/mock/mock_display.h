#pragma once

#include <lib_coffee_machine/interfaces.h>

class MockDisplay : public BaseDisplay
{
  public:
    MockDisplay() = default;

    bool initialise() override
    {
        return true;
    }
    bool clear() override
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const int &data) override
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const float &data) override
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const double &data) override
    {
        return true;
    }
};
