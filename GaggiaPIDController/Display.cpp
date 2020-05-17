#include "Display.h"

Display::Display() : display(), time_last_update(0), disable_display(false)
{
    Wire.begin();
    Wire.setClock(400000L);
    display.begin(&Adafruit128x64, SCREEN_I2C_ADDR);
    display.setFont(Adafruit5x7);
    //display.set2X();
    display.clear();
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
        write_machine_mode(status->machine_mode);
        write_current_temp(status->current_temperature);
        write_target_temp(status->target_temperature);
        write_status_message(status->status_message);
        time_last_update = now;
    }

    return true;
}

void Display::write_machine_mode(const Gaggia::MODE &mode)
{
    display.setCursor(30, 1);
    display.print((mode == Gaggia::WATER_MODE) ? "Water" : "Steam");
    display.print(" mode");
    display.clearToEOL();
    display.println();
}

void Display::write_current_temp(const double &temp)
{
    display.setCursor(1, 3);
    String str = String(temp, 1) + " C  ";
    display.print(str.c_str());
    display.println();
}

void Display::write_target_temp(const double &temp)
{
    display.setCursor(85, 3);
    String str = String(temp, 1) + " C  ";
    display.print(str.c_str());
    display.println();
}

void Display::write_status_message(const String &message)
{
    display.setCursor(1, 5);
    // TODO if the message is too long, make it scroll
    display.print(message.c_str());
    display.clearToEOL();
    display.println();
}
