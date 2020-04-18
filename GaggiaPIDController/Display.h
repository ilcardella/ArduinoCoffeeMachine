#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

class Display
{
  public:
    Display();
    bool update(Gaggia::ControlStatus *status);

  private:
    void write_machine_mode(const Gaggia::MODE &mode);
    void write_current_temp(const double &temp);
    void write_target_temp(const double &temp);
    void write_status_message(const String &message);

    static constexpr uint16_t REFRESH_PERIOD = 200;

    Adafruit_SSD1306 display;
    unsigned long time_last_update;
    bool disable_display;
};
