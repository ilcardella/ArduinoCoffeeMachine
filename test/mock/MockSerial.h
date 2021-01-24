#pragma once

#include <coffee_machine/BaseTypes.h>
#include <coffee_machine/Common.h>

template <class Adapter> class MockSerial : public BaseSerialInterface
{
  public:
    MockSerial() = default;

    void read_input() override
    {
        read_input_called = true;
    }

    void print_status(const Gaggia::ControlStatus &status) override
    {
        status_to_send = status;
    }

    bool is_debug_active() override
    {
        return debug_on;
    }

    double get_mock_temperature() override
    {
        return 42.0;
    }

    bool get_new_kp(double *kp) override
    {
        *kp = kp_in;
        return true;
    }

    bool get_new_ki(double *ki) override
    {
        *ki = ki_in;
        return true;
    }

    bool get_new_kd(double *kd) override
    {
        *kd = kd_in;
        return true;
    }

    void reset()
    {
        read_input_called = false;
        status_to_send = Gaggia::ControlStatus();
        debug_on = false;
        kp_in = 1.0;
        ki_in = 1.0;
        kd_in = 1.0;
    }

    bool read_input_called = false;
    Gaggia::ControlStatus status_to_send;
    bool debug_on = false;
    double kp_in = 1.0;
    double ki_in = 1.0;
    double kd_in = 1.0;
};
