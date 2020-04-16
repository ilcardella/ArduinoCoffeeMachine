#include "Display.h"

Display::Display() : time_last_update(0)
{
    // TODO initialise display
}

void Display::update(Gaggia::ControlStatus *status)
{
    auto now = millis();
    if (now - time_last_update < REFRESH_PERIOD)
    {
        // TODO update display
    }
}
