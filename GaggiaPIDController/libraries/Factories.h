#pragma once

#include <PID_v1.h>

#include "ArduinoSerial.h"
#include "SSD1306AsciiDisplay.h"
#include "Sensors.h"

#include "coffee_machine/BaseTypes.h"
#include "coffee_machine/Configuration.h"

class SensorFactory
{
  public:
    template <class Adapter, SensorTypes type>
    static BaseTemperatureSensor *make_temperature_sensor(const char *name,
                                                          const unsigned char &pin)
    {
        using namespace sensors;
        switch (type)
        {
        case SensorTypes::TSIC:
            return new TemperatureSensor<Adapter, TSICTempSensor>(name, pin, 300, 10);
            break;
        case SensorTypes::KTYPE_SPI:
            return new TemperatureSensor<Adapter, KTypeThermocouple>(name, pin, 300, 10,
                                                                     -12.0f);
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

class PIDFactory
{
  public:
    template <class Adapter> static BasePIDController *make_pid_controller()
    {
        return new RelayPIDController<Adapter, PID>(
            Configuration::P_GAIN, Configuration::I_GAIN, Configuration::D_GAIN);
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
