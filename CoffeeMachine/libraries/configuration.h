#pragma once

#include "lib_coffee_machine/include/lib_coffee_machine/default/configuration.h"

struct Configuration : public DefaultConfiguration
{
    enum class SensorTypes
    {
        TSIC,     // TSic sensor family
        KTYPE_SPI // K-type thermocouple with SPI interface
    };

    enum class DisplayTypes
    {
        SSD1306_128x64
    };

    enum class SerialTypes
    {
        ARDUINO_SERIAL
    };

    enum class TempControllerTypes
    {
        ARDUINO_PID
    };

    inline static constexpr SensorTypes WATER_TEMP_SENSOR_TYPE = SensorTypes::KTYPE_SPI;
    inline static constexpr SensorTypes STEAM_TEMP_SENSOR_TYPE = SensorTypes::KTYPE_SPI;

    inline static constexpr DisplayTypes DISPLAY_TYPE = DisplayTypes::SSD1306_128x64;

    inline static constexpr SerialTypes SERIAL_INTERFACE_TYPE =
        SerialTypes::ARDUINO_SERIAL;

    inline static constexpr TempControllerTypes TEMP_CONTROLLER_TYPE =
        TempControllerTypes::ARDUINO_PID;

    // Input pin of the water temperature sensor
    inline static constexpr unsigned char WATER_TEMP_PIN = 4;
    // Input pin of the steam temperature sensor
    inline static constexpr unsigned char STEAM_TEMP_PIN = 5;

    // Output PWM pin to control the boiler
    inline static constexpr unsigned char HEATER_SSR_PIN = 6;

    // Input pin to detect steam mode
    inline static constexpr unsigned char STEAM_SWITCH_PIN = 7;

    // SPI interface common pins
    inline static constexpr unsigned char SPI_CLK_PIN = 8;
    inline static constexpr unsigned char SPI_CS_PIN = 9;
};
