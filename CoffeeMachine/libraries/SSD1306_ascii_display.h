#pragma once

#include "lib_coffee_machine/include/lib_coffee_machine/interfaces.h"

#include <SSD1306AsciiWire.h>
#include <Wire.h>

class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay()
    {
    }

    bool initialise() override
    {
        Wire.begin();
        Wire.setClock(400000L);
        oled.begin(&Adafruit128x64, 0x3C);
        oled.setFont(Adafruit5x7);
        clear();
        return true;
    }

    bool clear() override
    {
        oled.clear();
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
        oled.setCursor(col, row);
        oled.print(data);
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

    bool display()
    {
        // Nothing to do
        return true;
    }

  private:
    SSD1306AsciiWire oled;
};
