#pragma once

#include <coffee_machine/BaseTypes.h>

class MockDisplay : public BaseDisplay
{
  public:
    MockDisplay() = default;

    bool initialise()
    {
        return true;
    }
    bool clear()
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const char *data)
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const int &data)
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const float &data)
    {
        return true;
    }
    bool print(const unsigned &col, const unsigned &row, const double &data)
    {
        return true;
    }
};
