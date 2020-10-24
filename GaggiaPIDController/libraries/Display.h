#pragma once

#include "coffee_machine/BaseTypes.h"
#include "coffee_machine/Common.h"

template <class Adapter, class DisplayType> class Display : public BaseDisplay<Adapter>
{
  public:
    Display() : display(), time_last_update(0)
    {
        Adapter::WireBegin();
        Adapter::WireSetClock(400000L);
        display.begin(&Adafruit128x64, 0x3C);
        display.setFont(Adafruit5x7);
        display.clear();
    }

    bool update(const Gaggia::ControlStatus<Adapter> &status) override
    {
        // Refresh the display with a reasonable rate
        auto now = Adapter::millis();
        if (now - time_last_update > REFRESH_PERIOD)
        {
            write_machine_mode(status.machine_mode);
            write_current_temp(status.current_temperature);
            write_target_temp(status.target_temperature);
            write_status_message(status.status_message);
            time_last_update = now;
        }

        return true;
    }

  private:
    void write_machine_mode(const Gaggia::Mode &mode)
    {
        display.setCursor(30, 1);
        display.print((mode == Gaggia::Mode::WATER_MODE) ? "Water" : "Steam");
        display.print(" mode");
        display.clearToEOL();
        display.println();
    }

    void write_current_temp(const double &temp)
    {
        display.setCursor(1, 3);
        typename Adapter::String str = typename Adapter::String(temp, 1) + " C  ";
        display.print(str.c_str());
        display.println();
    }

    void write_target_temp(const double &temp)
    {
        display.setCursor(85, 3);
        typename Adapter::String str = typename Adapter::String(temp, 1) + " C  ";
        display.print(str.c_str());
        display.println();
    }

    void write_status_message(const typename Adapter::String &message)
    {
        display.setCursor(1, 5);
        // TODO if the message is too long, make it scroll
        display.print(message.c_str());
        display.clearToEOL();
        display.println();
    }

    static constexpr unsigned short REFRESH_PERIOD = 200;

    DisplayType display;
    unsigned long time_last_update;
};
