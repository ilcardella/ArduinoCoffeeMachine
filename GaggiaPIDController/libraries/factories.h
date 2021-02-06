#pragma once

#include "SSD1306_ascii_display.h"
#include "arduino_pid.h"
#include "arduino_serial.h"
#include "ktype_thermocouple.h"
#include "tsic_sensor.h"

#include "coffee_machine/interfaces.h"

class SensorFactory
{
  public:
    template <class Configuration> static BaseSensor *make_water_temperature_sensor()
    {
        return make_temperature_sensor<Configuration>(
            Configuration::WATER_TEMP_SENSOR_TYPE, Configuration::WATER_TEMP_PIN);
    }

    template <class Configuration> static BaseSensor *make_steam_temperature_sensor()
    {
        return make_temperature_sensor<Configuration>(
            Configuration::STEAM_TEMP_SENSOR_TYPE, Configuration::STEAM_TEMP_PIN);
    }

  private:
    template <class Configuration>
    static BaseSensor *make_temperature_sensor(typename Configuration::SensorTypes type,
                                               const unsigned char &pin)
    {
        switch (type)
        {
        case Configuration::SensorTypes::TSIC:
            return new TSICTempSensor(pin);
            break;
        case Configuration::SensorTypes::KTYPE_SPI:
            return new KTypeThermocouple(pin, Configuration::SPI_CLK_PIN,
                                         Configuration::SPI_DO_PIN);
            break;
        default:
            // Ideally we would raise an exception here
            return nullptr;
            break;
        }
    }
};

class DisplayFactory
{
  public:
    template <class Adapter, class Configuration> static BaseDisplay *make_display()
    {
        switch (Configuration::DISPLAY_TYPE)
        {
        case Configuration::DisplayTypes::SSD1306_128x64:
            return new SSD1306AsciiDisplay<Adapter>();
            break;
        default:
            // Ideally we would raise an exception here
            return nullptr;
            break;
        }
    }
};

class ControllerFactory
{
  public:
    template <class Configuration> static Controller *make_controller()
    {
        switch (Configuration::TEMP_CONTROLLER_TYPE)
        {
        case Configuration::TempControllerTypes::ARDUINO_PID:
            return new ArduinoPID(Configuration::P_GAIN, Configuration::I_GAIN,
                                  Configuration::D_GAIN);
            break;
        default:
            // Ideally we would raise an exception here
            return nullptr;
            break;
        }
    }
};

class SerialFactory
{
  public:
    template <class Adapter, class Configuration>
    static BaseSerialInterface *make_serial_interface()
    {
        switch (Configuration::SERIAL_INTERFACE_TYPE)
        {
        case Configuration::SerialTypes::ARDUINO_SERIAL:
            return new ArduinoSerial<Adapter>(Configuration::SERIAL_BAUDRATE);
            break;
        default:
            // Ideally we would raise an exception here
            return nullptr;
            break;
        }
    }
};
