#pragma once

#include "BaseTypes.h"
#include "Common.h"

template <class Adapter> class Display
{
  public:
    Display(BaseDisplay *display) : display(display), time_last_update(0)
    {
    }

    bool update(const Gaggia::ControlStatus<Adapter> &status)
    {
        // Refresh the display with a reasonable rate
        auto now = Adapter::millis();
        if (now - time_last_update > REFRESH_PERIOD)
        {
            display->clear();
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
        display->print(30, 1,
                       (mode == Gaggia::Mode::WATER_MODE) ? "Water mode" : "Steam mode");
    }

    void write_current_temp(const double &temp)
    {
        write_temp(1, 3, temp);
    }

    void write_target_temp(const double &temp)
    {
        write_temp(85, 3, temp);
    }

    void write_temp(const unsigned &col, const unsigned &row, const double &temp)
    {
        char output[10];
        char buffer[6];
        string_utils::my_dtostrf(temp, 4, 1, buffer);
        snprintf(output, 10, "%s C  ", buffer);
        display->print(col, row, output);
    }

    void write_status_message(const char *message)
    {
        // TODO if the message is too long, make it scroll
        display->print(1, 5, message);
    }

    static constexpr unsigned short REFRESH_PERIOD = 200;

    BaseDisplay *display;
    unsigned long time_last_update;
};
