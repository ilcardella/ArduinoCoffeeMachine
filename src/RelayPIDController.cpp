#include "RelayPIDController.h"

RelayPIDController::RelayPIDController(const uint16_t &kp, const uint16_t &ki,
                                       const uint16_t &kd)
    : p_gain(kp), i_gain(ki), d_gain(kd), pid_input(0.0), pid_output(0.0),
      pid_setpoint(0.0), pid_window_size(5000), pid_window_start(millis())
{
    pid = new PID(&pid_input, &pid_output, &pid_setpoint, kp, ki, kd, DIRECT);
    pid->SetOutputLimits(0, pid_window_size);
    pid->SetMode(AUTOMATIC);
}

bool RelayPIDController::compute(double *input, double *setpoint)
{
    // Update the pid wrapper status
    pid_input = *input;
    pid_setpoint = *setpoint;

    // Process the pid feedback loop
    pid->Compute();

    // Check if the window needs to be shifted
    if (millis() - pid_window_start > pid_window_size)
    {
        pid_window_start += pid_window_size;
    }

    // Return a digitail status based on the pid output
    if (pid_output < millis() - pid_window_start)
    {
        return true;
    }
    return false;
}
