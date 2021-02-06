#pragma once

#include <coffee_machine/interfaces.h>

class MockIOPin : public IOPin
{
  public:
    MockIOPin() = default;

    void set_mode(const Modes &mode) override
    {
    }

    bool is_high() override
    {
        return pin_status;
    }

    bool is_low() override
    {
        return pin_status;
    }

    void digital_write_high() override
    {
    }

    void digital_write_low() override
    {
    }

    void set_water_mode()
    {
        pin_status = true;
    }

    void set_steam_mode()
    {
        pin_status = false;
    }

    bool pin_status = false;
};
