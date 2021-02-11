#pragma once

#include <lib_coffee_machine/interfaces.h>

class MockController : public Controller
{
  public:
    MockController() = default;

    void set_output_limits(const double &min, const double &max) override
    {
        min_output = min;
        max_output = max;
    }

    bool update_settings(char *raw) override
    {
        return true;
    }

    bool compute(const double &input, const double &setpoint, double &output) override
    {
        output = mock_output;
        return healthy;
    }

    void set_max_output()
    {
        mock_output = max_output;
    }

    void set_min_output()
    {
        mock_output = min_output;
    }

    bool healthy = true;
    double mock_output = 0.0;
    double max_output = 0.0;
    double min_output = 0.0;
};
