#pragma once

#include "coffee_machine/BaseTypes.h"

template <class Adapter, class PIDType>
class RelayPIDController : public BasePIDController
{
  public:
    RelayPIDController(const double &kp, const double &ki, const double &kd)
        : pid_input(0.0), pid_output(0.0), pid_setpoint(0.0), pid_window_size(5000),
          p_gain(kp), i_gain(ki), d_gain(kd),
          pid(&pid_input, &pid_output, &pid_setpoint, p_gain, i_gain, d_gain, DIRECT)
    {
        // the PID output will represents a time window capped at pid_window_size
        pid.SetOutputLimits(0, pid_window_size);
        pid.SetMode(AUTOMATIC);
    }

    bool compute(const double &input, const double &setpoint, bool *relay_on) override
    {
        // How much time it has passed in the current window frame
        unsigned short window_progress = Adapter::millis() % pid_window_size;

        // Update the pid input and setpoint with those requested
        pid_input = input;
        pid_setpoint = setpoint;

        // Process the pid output
        pid.Compute();

        // Set the relay status based on the pid output
        *relay_on = (pid_output >= window_progress);

        return true;
    }

    void set_kp(const double &kp) override
    {
        p_gain = kp;
        pid.SetTunings(p_gain, i_gain, d_gain);
    }

    void set_ki(const double &ki) override
    {
        i_gain = ki;
        pid.SetTunings(p_gain, i_gain, d_gain);
    }

    void set_kd(const double &kd) override
    {
        d_gain = kd;
        pid.SetTunings(p_gain, i_gain, d_gain);
    }

  private:
    PIDType pid;
    double p_gain;
    double i_gain;
    double d_gain;

    double pid_input;
    double pid_output;
    double pid_setpoint;
    unsigned short pid_window_size;
};
