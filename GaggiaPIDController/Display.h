#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

class Display
{
  public:
    Display();
    void update(Gaggia::ControlStatus *status);

  private:
    static constexpr uint16_t REFRESH_PERIOD = 200;
    unsigned long time_last_update;
};
