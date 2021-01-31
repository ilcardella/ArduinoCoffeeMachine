#pragma once

#include "coffee_machine/interfaces.h"

#include <SSD1306AsciiWire.h>

template <class Adapter> class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay()
    {
    }

    bool initialise() override
    {
        Adapter::WireBegin();
        Adapter::WireSetClock(400000L);
        display.begin(&Adafruit128x64, 0x3C);
        display.setFont(Adafruit5x7);
        clear();
        return true;
    }

    bool clear() override
    {
        display.clear();
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
        display.setCursor(col, row);
        display.print(data);
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const int &data) override
    {
        // TODO
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const float &data) override
    {
        // TODO
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const double &data) override
    {
        // TODO
        return true;
    }

  private:
    SSD1306AsciiWire display;
};
