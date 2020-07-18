#pragma once

#include <Arduino.h>

namespace Gaggia
{
enum MODE
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

struct ControlStatus
{
    MODE machine_mode = MODE::WATER_MODE;
    double current_temperature = 0.0;
    double target_temperature = 0.0;
    bool water_heater_on = false;
    String status_message = "Loading...";
    unsigned long time_since_start = 0;
    unsigned long time_since_steam_mode = 0;
};
} // namespace Gaggia
