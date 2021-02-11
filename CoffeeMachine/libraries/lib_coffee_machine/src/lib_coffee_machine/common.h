#pragma once

namespace Machine
{
enum class Mode
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

struct Status
{
    Mode machine_mode = Mode::WATER_MODE;
    double current_temperature = 0.0;
    double target_temperature = 0.0;
    bool water_heater_on = false;
    static constexpr unsigned MSG_LEN = 100;
    char status_message[MSG_LEN];
    unsigned long start_timestamp = 0;
    unsigned long steam_mode_timestamp = 0;
};
} // namespace Machine

namespace string_utils
{
namespace strings
{
inline static constexpr char LOADING[] = "Loading...";
inline static constexpr char HEATING[] = "Heating...";
inline static constexpr char COOLING[] = "Cooling...";
inline static constexpr char READY[] = "Ready";
inline static constexpr char DEBUG_MODE[] = "Debug mode";
inline static constexpr char PID_FAULT[] = "PID fault";
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

} // namespace string_utils