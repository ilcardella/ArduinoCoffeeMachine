#include "RelayPIDController.h"

RelayPIDController::RelayPIDController(const uint16_t &kp, const uint16_t &ki,
                                       const uint16_t &kd)
    : pid_input(0.0), pid_output(0.0), pid_setpoint(0.0), pid_window_size(5000),
      p_gain(kp), i_gain(ki), d_gain(kd)
{
    pid = new PID(&pid_input, &pid_output, &pid_setpoint, p_gain, i_gain, d_gain, DIRECT);
    // the PID output will represents a time window capped at pid_window_size
    pid->SetOutputLimits(0, pid_window_size);
    pid->SetMode(AUTOMATIC);
}

bool RelayPIDController::compute(double *input, double *setpoint, bool *relay_on)
{
    // How much time it has passed in the current window frame
    uint16_t window_progress = millis() % pid_window_size;

    // Update the pid input and setpoint with those requested
    pid_input = *input;
    pid_setpoint = *setpoint;

    // Process the pid output
    pid->Compute();

    // Set the relay status based on the pid output
    *relay_on = (pid_output >= window_progress) ? true : false;

    return true;
}

void RelayPIDController::set_kp(uint16_t *kp)
{
    p_gain = *kp;
    pid->SetTunings(p_gain, i_gain, d_gain);
}

void RelayPIDController::set_ki(uint16_t *ki)
{
    i_gain = *ki;
    pid->SetTunings(p_gain, i_gain, d_gain);
}

void RelayPIDController::set_kd(uint16_t *kd)
{
    d_gain = *kd;
    pid->SetTunings(p_gain, i_gain, d_gain);
}
