#pragma once

#include <Arduino.h>
#include <PID_v1.h>

class RelayPIDController
{
  public:
    RelayPIDController(const uint16_t &kp, const uint16_t &ki, const uint16_t &kd);

    bool compute(double *input, double *setpoint, bool *relay_on);

    void set_kp(uint16_t *kp);
    void set_ki(uint16_t *ki);
    void set_kd(uint16_t *kd);

  private:
    PID *pid;
    uint16_t p_gain;
    uint16_t i_gain;
    uint16_t d_gain;

    double pid_input;
    double pid_output;
    double pid_setpoint;
    uint16_t pid_window_size;
};
