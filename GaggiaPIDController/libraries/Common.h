#pragma once

#include <Arduino.h>

namespace Gaggia
{
enum MODE
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

struct ControlStatus
{
    MODE machine_mode = MODE::WATER_MODE;
    double current_temperature = 0.0;
    double target_temperature = 0.0;
    bool water_heater_on = false;
    String status_message = "Loading...";
    unsigned long time_since_start = 0;
    unsigned long time_since_steam_mode = 0;
};
} // namespace Gaggia

template <typename T> class MovingAverage
{
  public:
    MovingAverage(const uint32_t &window_size) : window_size(window_size)
    {
        readings = new T[window_size];
        reset();
    }

    ~MovingAverage()
    {
        delete readings;
        readings = nullptr;
    }

    T add(const T &value)
    {
        sum -= readings[index];
        readings[index] = value;
        sum += value;
        index = (++index) % window_size;
    }

    T get()
    {
        return sum / window_size;
    }

    void reset()
    {
        index = 0;
        sum = 0;
        for (unsigned i = 0; i < window_size; ++i)
        {
            readings[i] = 0;
        }
    }

  private:
    uint32_t window_size;
    T *readings;
    uint32_t index;
    T sum;
};
