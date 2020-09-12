#pragma once

#include <coffee_machine/BaseTypes.h>
#include <coffee_machine/Common.h>

template <class Adapter> class MockSerial : public BaseSerialInterface<Adapter>
{
  public:
    MockSerial() = default;

    void read_input() override
    {
    }

    void print_status(const Gaggia::ControlStatus<Adapter> &status) override
    {
    }

    bool is_debug_active() override
    {
        return false;
    }

    double get_mock_temperature() override
    {
        return 42.0;
    }

    bool get_new_kp(double *kp) override
    {
        *kp = 1.0;
        return true;
    }

    bool get_new_ki(double *ki) override
    {
        *ki = 1.0;
        return true;
    }

    bool get_new_kd(double *kd) override
    {
        *kd = 1.0;
        return true;
    }

    void reset()
    {
        // TODO
    }

    // TODO
};
