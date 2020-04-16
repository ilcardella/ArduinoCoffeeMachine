#pragma once

#include <Arduino.h>
#include <PID_v1.h>

class RelayPIDController
{
  public:
    RelayPIDController(const uint16_t &kp, const uint16_t &ki, const uint16_t &kd);

    bool compute(double *input, double *setpoint, bool *relay_on);

  private:
    PID *pid;

    double pid_input;
    double pid_output;
    double pid_setpoint;
    uint16_t pid_window_size;
    uint16_t pid_window_start;
};