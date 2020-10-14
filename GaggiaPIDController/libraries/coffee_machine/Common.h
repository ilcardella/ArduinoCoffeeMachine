#pragma once

namespace Gaggia
{
enum class Mode
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

template <class Adapter> struct ControlStatus
{
    Mode machine_mode = Mode::WATER_MODE;
    double current_temperature = 0.0;
    double target_temperature = 0.0;
    bool water_heater_on = false;
    typename Adapter::String status_message = "Loading...";
    unsigned long time_since_start = 0;
    unsigned long time_since_steam_mode = 0;
};
} // namespace Gaggia

template <typename T> class MovingAverage
{
  public:
    MovingAverage(const unsigned int &window_size)
        : window_size(window_size), index(0), sum(0)
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
    unsigned int window_size;
    T *readings;
    unsigned int index;
    T sum;
};
