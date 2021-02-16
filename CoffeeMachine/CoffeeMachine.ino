#include "libraries/arduino_adapter.h"
#include "libraries/arduino_pin.h"
#include "libraries/factories.h"

#include "libraries/lib_coffee_machine/include/lib_coffee_machine/coffee_machine.h"
#include "libraries/lib_coffee_machine/include/lib_coffee_machine/default_configuration.h"

using Adapter = ArduinoAdapter;
using Configuration = DefaultConfiguration;

CoffeeMachine<Adapter, Configuration> *machine;

void setup()
{
    auto display = DisplayFactory::make_display<Configuration>();
    auto serial = SerialFactory::make_serial_interface<Configuration>();
    auto controller = ControllerFactory::make_controller<Configuration>();
    auto mode_swith_pin = new ArduinoPin(Configuration::STEAM_SWITCH_PIN);
    auto heater_pin = new ArduinoPin(Configuration::HEATER_SSR_PIN);
    auto water_sensor = SensorFactory::make_water_temperature_sensor<Configuration>();
    auto steam_sensor = SensorFactory::make_steam_temperature_sensor<Configuration>();

    machine = new CoffeeMachine<Adapter, Configuration>(
        controller, serial, mode_swith_pin, display, heater_pin, water_sensor,
        steam_sensor);

    // Allow sensors to initialise
    delay(500);
}

void loop()
{
    if (machine)
    {
        machine->spin();
    }
    else
    {
        Serial.println("Error initialising coffee machine");
        delay(1000);
    }
}
