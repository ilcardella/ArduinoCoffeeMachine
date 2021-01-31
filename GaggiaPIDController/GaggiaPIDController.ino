#include "libraries/arduino_adapter.h"
#include "libraries/arduino_pin.h"
#include "libraries/factories.h"

#include "libraries/coffee_machine/coffee_machine.h"
#include "libraries/coffee_machine/interfaces.h"

using Adapter = ArduinoAdapter;

CoffeeMachine<Adapter> *machine;

void setup()
{
    auto display = DisplayFactory::make_display<Adapter, Configuration::DISPLAY_TYPE>();
    auto serial =
        SerialFactory::make_serial_interface<Adapter,
                                             Configuration::SERIAL_INTERFACE_TYPE>();
    auto controller =
        ControllerFactory::make_controller<Adapter,
                                           Configuration::TEMP_CONTROLLER_TYPE>();
    auto mode_swith_pin = new ArduinoPin(Configuration::STEAM_SWITCH_PIN);
    auto heater_pin = new ArduinoPin(Configuration::HEATER_SSR_PIN);
    auto water_sensor =
        SensorFactory::make_temperature_sensor<Adapter,
                                               Configuration::WATER_TEMP_SENSOR_TYPE>(
            "water_sensor", Configuration::WATER_TEMP_PIN);
    auto steam_sensor =
        SensorFactory::make_temperature_sensor<Adapter,
                                               Configuration::STEAM_TEMP_SENSOR_TYPE>(
            "steam_sensor", Configuration::STEAM_TEMP_PIN);

    machine = new CoffeeMachine<Adapter>(controller, serial, mode_swith_pin, display,
                                         heater_pin, water_sensor, steam_sensor);

    // Allow sensors to initialise
    Adapter::delay(500);
}

void loop()
{
    if (machine)
    {
        machine->spin();
    }
    else
    {
        Adapter::SerialPrintln("Error initialising coffee machine");
        Adapter::delay(1000);
    }
}
