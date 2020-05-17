#pragma once

#include "libraries/Common.h"
#include <Arduino.h>

struct SerialInput
{
    bool debug_mode = false;
    bool enable_output = true;
    double mock_temperature = 0.0;
    uint16_t kp = 0;
    uint16_t ki = 0;
    uint16_t kd = 0;
};

class SerialInterface
{
  public:
    SerialInterface(const uint16_t &baudrate);

    void read_input();

    void print_status(Gaggia::ControlStatus *status);
    void print_help();

    bool is_debug_active();
    double get_mock_temperature();
    bool is_output_enabled();
    bool get_new_kp(uint16_t *kp);
    bool get_new_ki(uint16_t *ki);
    bool get_new_kd(uint16_t *kd);

  private:
    static constexpr uint16_t PRINT_TIMEOUT = 200;
    unsigned long time_last_print;
    SerialInput inputs;
};
