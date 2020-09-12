#pragma once

#include <Arduino.h>
#include <PID_v1.h>

class RelayPIDController
{
  public:
    RelayPIDController(const double &kp, const double &ki, const double &kd);

    bool compute(const double &input, const double &setpoint, bool *relay_on);

    void set_kp(const double &kp);
    void set_ki(const double &ki);
    void set_kd(const double &kd);

  private:
    PID *pid;
    double p_gain;
    double i_gain;
    double d_gain;

    double pid_input;
    double pid_output;
    double pid_setpoint;
    unsigned short pid_window_size;
};
