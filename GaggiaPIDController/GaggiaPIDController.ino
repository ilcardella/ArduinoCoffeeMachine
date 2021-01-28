#include "libraries/ArduinoAdapter.h"
#include "libraries/Factories.h"

#include "libraries/coffee_machine/BaseTypes.h"
#include "libraries/coffee_machine/CoffeeMachine.h"
#include "libraries/coffee_machine/Display.h"
#include "libraries/coffee_machine/Heater.h"
#include "libraries/coffee_machine/ModeDetector.h"
#include "libraries/coffee_machine/RelayPIDController.h"
#include "libraries/coffee_machine/SerialInterface.h"

using Adapter = ArduinoAdapter;

CoffeeMachine<Adapter> *machine;

void setup()
{
    auto display = DisplayFactory::make_display<Adapter, Configuration::DISPLAY_TYPE>();
    auto serial = new SerialInterface<Adapter>(Configuration::SERIAL_BAUDRATE);
    auto pid = PIDFactory::make_pid_controller<Adapter>();
    auto mode_detector = new ModeDetector<Adapter>(Configuration::STEAM_SWITCH_PIN);
    auto heater = new Heater<Adapter>(Configuration::HEATER_SSR_PIN);
    auto water_sensor =
        SensorFactory::make_temperature_sensor<Adapter,
                                               Configuration::WATER_TEMP_SENSOR_TYPE>(
            "water_sensor", Configuration::WATER_TEMP_PIN);
    auto steam_sensor =
        SensorFactory::make_temperature_sensor<Adapter,
                                               Configuration::STEAM_TEMP_SENSOR_TYPE>(
            "steam_sensor", Configuration::STEAM_TEMP_PIN);

    machine = new CoffeeMachine<Adapter>(pid, serial, mode_detector, display, heater,
                                         water_sensor, steam_sensor);

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
