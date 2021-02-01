#pragma once

#include "SSD1306_ascii_display.h"
#include "arduino_pid.h"
#include "arduino_serial.h"
#include "ktype_thermocouple.h"
#include "tsic_sensor.h"

#include "coffee_machine/configuration.h"
#include "coffee_machine/interfaces.h"

class SensorFactory
{
  public:
    template <SensorTypes type>
    static BaseSensor *make_temperature_sensor(const unsigned char &pin)
    {
        switch (type)
        {
        case SensorTypes::TSIC:
            return new TSICTempSensor(pin);
            break;
        case SensorTypes::KTYPE_SPI:
            return new KTypeThermocouple(pin);
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
    template <class Adapter, DisplayTypes type> static BaseDisplay *make_display()
    {
        switch (type)
        {
        case DisplayTypes::SSD1306_128x64:
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
    template <TempControllerTypes type> static Controller *make_controller()
    {
        switch (type)
        {
        case TempControllerTypes::ARDUINO_PID:
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
    template <class Adapter, SerialTypes type>
    static BaseSerialInterface *make_serial_interface()
    {
        switch (type)
        {
        case SerialTypes::ARDUINO_SERIAL:
            return new ArduinoSerial<Adapter>(Configuration::SERIAL_BAUDRATE);
            break;
        default:
            // Ideally we would raise an exception here
            return nullptr;
            break;
        }
    }
};
