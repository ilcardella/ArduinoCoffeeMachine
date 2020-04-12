#include "RelayPIDController.h"

RelayPIDController::RelayPIDController(const uint16_t &kp, const uint16_t &ki,
                                       const uint16_t &kd)
    : p_gain(kp), i_gain(ki), d_gain(kd), pid_input(0.0), pid_output(0.0),
      pid_setpoint(0.0), pid_window_size(5000), pid_window_start(millis())
{
    pid = new PID(&pid_input, &pid_output, &pid_setpoint, kp, ki, kd, DIRECT);
    // the PID output will represents a time window capped at pid_window_size
    pid->SetOutputLimits(0, pid_window_size);
    pid->SetMode(AUTOMATIC);
}

bool RelayPIDController::compute(double *input, double *setpoint)
{
    auto now = millis();

    // How much time it has passed in the current window frame
    long window_progress = now - pid_window_start;

    // Restart the window then millis() rolls over
    if (window_progress < 0)
    {
        pid_window_start = 0;
        window_progress = now - pid_window_start;
    }

    // Update the pid input and setpoint with those requested
    pid_input = *input;
    pid_setpoint = *setpoint;

    // Process the pid output
    pid->Compute();

    // Check if the window needs to be shifted
    if (window_progress > pid_window_size)
    {
        pid_window_start += pid_window_size;
    }

    // Return a digital status based on the pid output
    if (pid_output < window_progress)
    {
        return false;
    }
    return true;
}
