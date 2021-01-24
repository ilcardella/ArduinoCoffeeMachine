#pragma once

namespace Gaggia
{
enum class Mode
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

struct ControlStatus
{
    Mode machine_mode = Mode::WATER_MODE;
    double current_temperature = 0.0;
    double target_temperature = 0.0;
    bool water_heater_on = false;
    static constexpr unsigned MSG_LEN = 100;
    char status_message[MSG_LEN];
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

namespace string_utils
{
namespace strings
{
inline static constexpr char LOADING[] = "Loading...";
inline static constexpr char HEATING[] = "Heating...";
inline static constexpr char COOLING[] = "Cooling...";
inline static constexpr char READY[] = "Ready";
inline static constexpr char DEBUG_MODE[] = "Debug mode";
inline static constexpr char PID_FAILT[] = "PID fault";
inline static constexpr char STEAM_TIMEOUT_EXPIRED[] = "Steam mode timeout expired";
inline static constexpr char SAFETY_TIMEOUT_EXPIRED[] = "Safety timeout expired";
inline static constexpr char TEMP_SENSOR_ERROR[] = "Temperature sensor fault";

} // namespace strings

bool start_with(const char *data, const char *prefix)
{
    return strlen(prefix) <= strlen(data) && (strncmp(data, prefix, strlen(prefix)) == 0);
}

template <typename T> T to_number(const char *data, const unsigned &start_index = 0)
{
    char number[strlen(data) - start_index + 1];
    strcpy(number, data + start_index);
    return static_cast<T>(atof(number));
}

char *my_dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
    char fmt[20];
    sprintf(fmt, "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
}
} // namespace string_utils