#pragma once

#include <coffee_machine/BaseTypes.h>

class MockPID : public BasePIDController
{
  public:
    MockPID() = default;
    bool compute(const double &input, const double &setpoint, bool *relay_on) override
    {
        *relay_on = relay;
        return healthy;
    }

    void set_kp(const double &kp) override
    {
        this->kp = kp;
    }

    void set_ki(const double &ki) override
    {
        this->ki = ki;
    }

    void set_kd(const double &kd) override
    {
        this->kd = kd;
    }

    void reset()
    {
        healthy = true;
        relay = false;
        kp = 0.0;
        ki = 0.0;
        kd = 0.0;
    }

    bool healthy = true;
    bool relay = false;
    double kp = 0.0;
    double ki = 0.0;
    double kd = 0.0;
};
