#include "Display.h"

Display::Display()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), time_last_update(0),
      disable_display(false)
{
    // Initialise the dipslay and show the Adafruit splashscreen
    // If initialisation fails, do not do anything
    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        display.setTextSize(1);              // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE); // Draw white text
        display.cp437(true);                 // Use full 256 char 'Code Page 437' font
        display.display();                   // Show splashscreen
    }
    else
    {
        disable_display = true;
    }
}

bool Display::update(Gaggia::ControlStatus *status)
{
    // Do nothing if display was not initialised
    if (disable_display)
    {
        status->status_message = "Display not initialised";
        return false;
    }

    // Refresh the display with a reasonable rate
    auto now = millis();
    if (now - time_last_update > REFRESH_PERIOD)
    {
        display.clearDisplay();
        write_machine_mode(status->machine_mode);
        write_current_temp(status->current_temperature);
        write_target_temp(status->target_temperature);
        write_status_message(status->status_message);
        display.display();
        time_last_update = now;
    }

    return true;
}

void Display::write_machine_mode(const Gaggia::MODE &mode)
{
    display.setCursor(43, 1);
    display.write((mode == Gaggia::WATER_MODE) ? "Water" : "Steam");
}

void Display::write_current_temp(const double &temp)
{
    display.setCursor(1, 11);
    String str = String(temp, 1) + " C";
    display.write(str.c_str());
}

void Display::write_target_temp(const double &temp)
{
    display.setCursor(85, 11);
    String str = String(temp, 1) + " C";
    display.write(str.c_str());
}

void Display::write_status_message(const String &message)
{
    display.setCursor(1, 21);
    // TODO if the message is too long, make it scroll
    display.write(message.c_str());
}
