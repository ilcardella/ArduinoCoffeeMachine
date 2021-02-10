#pragma once

#include <PID_v1.h>

#include "lib_coffee_machine/src/lib_coffee_machine/interfaces.h"

class ArduinoPID : public Controller
{
  public:
    ArduinoPID(const double &kp, const double &ki, const double &kd)
        : p_gain(kp), i_gain(ki), d_gain(kd),
          pid(&pid_input, &pid_output, &pid_setpoint, p_gain, i_gain, d_gain, DIRECT)
    {
        pid.SetMode(AUTOMATIC);
    }

    void set_output_limits(const double &min, const double &max) override
    {
        pid.SetOutputLimits(min, max);
    }

    bool compute(const double &input, const double &setpoint, double &output) override
    {
        pid_input = input;
        pid_setpoint = setpoint;
        if (pid.Compute())
        {
            output = pid_output;
            return true;
        }
        return false;
    }

    bool update_settings(char *raw) override
    {
        // TODO decode spec and set tunings
        pid.SetTunings(0.0, 0.0, 0.0);
    }

  private:
    PID pid;
    double p_gain = 0.0;
    double i_gain = 0.0;
    double d_gain = 0.0;
    double pid_input = 0.0;
    double pid_output = 0.0;
    double pid_setpoint = 0.0;
};
