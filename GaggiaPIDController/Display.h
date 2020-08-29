#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <Wire.h>

#define SCREEN_I2C_ADDR 0x3C

class Display
{
  public:
    Display();
    bool update(const Gaggia::ControlStatus &status);

  private:
    void write_machine_mode(const Gaggia::MODE &mode);
    void write_current_temp(const double &temp);
    void write_target_temp(const double &temp);
    void write_status_message(const String &message);

    static constexpr uint16_t REFRESH_PERIOD = 200;

    SSD1306AsciiWire display;
    unsigned long time_last_update;
};
