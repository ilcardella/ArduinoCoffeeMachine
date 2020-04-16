#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

class SerialInterface
{
  public:
    SerialInterface(const uint16_t &baudrate);
    void read_input();
    void print_status(Gaggia::ControlStatus *status);
    bool is_debug_active();
    double get_mock_temperature();

  private:
    static constexpr uint16_t PRINT_TIMEOUT = 1000;
    unsigned long time_last_print;
    bool debug_mode;
    double mock_temperature;
};
