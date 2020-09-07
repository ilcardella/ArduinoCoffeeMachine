#pragma once

enum class SensorTypes
{
    TSIC,     // TSic sensor family
    KTYPE_SPI // K-type thermocouple with SPI interface
};

struct Configuration
{
    inline static constexpr SensorTypes WATER_TEMP_SENSOR_TYPE = SensorTypes::KTYPE_SPI;
    inline static constexpr SensorTypes STEAM_TEMP_SENSOR_TYPE = SensorTypes::KTYPE_SPI;
    // Input pin of the water temperature sensor
    inline static constexpr uint8_t WATER_TEMP_PIN = 4;
    // Input pin of the steam temperature sensor
    inline static constexpr uint8_t STEAM_TEMP_PIN = 5;

    // Output PWM pin to control the boiler
    inline static constexpr uint8_t HEATER_SSR_PIN = 6;

    // Input pin to detect steam mode
    inline static constexpr uint8_t STEAM_SWITCH_PIN = 7;

    // Target water temperature in celsius
    inline static constexpr double TARGET_WATER_TEMP = 95.0;
    // Target steam temperature in celsius
    inline static constexpr double TARGET_STEAM_TEMP = 150.0;

    // PID gain parameters
    inline static constexpr double P_GAIN = 125;
    inline static constexpr double I_GAIN = 0.8;
    inline static constexpr double D_GAIN = 0.75;

    // SPI interface common pins
    inline static constexpr uint8_t SPI_CLK_PIN = 8;
    inline static constexpr uint8_t SPI_DO_PIN = 9;

    // Serial interface
    inline static constexpr unsigned long SERIAL_BAUDRATE = 9600;

    // Safety timeouts in milliseconds to turn off the heater. (disabled if < 1)
    inline static constexpr unsigned long SAFETY_TIMEOUT = 40 * 60 * 1000; // 40 minutes
    inline static constexpr unsigned long STEAM_TIMEOUT = 5 * 60 * 1000;   // 5 minutes
};
