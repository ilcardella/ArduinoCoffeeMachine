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
Display<Adapter> *display;
BaseSerialInterface *serial;
BasePIDController *pid;
BaseModeDetector *mode_detector;
BaseHeater *heater;
BaseTemperatureSensor *water_sensor;
BaseTemperatureSensor *steam_sensor;

void setup()
{
    display = DisplayFactory::make_display<Adapter, Configuration::DISPLAY_TYPE>();
    serial = new SerialInterface<Adapter>(Configuration::SERIAL_BAUDRATE);
    pid = PIDFactory::make_pid_controller<Adapter>();
    mode_detector = new ModeDetector<Adapter>(Configuration::STEAM_SWITCH_PIN);
    heater = new Heater<Adapter>(Configuration::HEATER_SSR_PIN);
    water_sensor =
        SensorFactory::make_temperature_sensor<Adapter,
                                               Configuration::WATER_TEMP_SENSOR_TYPE>(
            "water_sensor", Configuration::WATER_TEMP_PIN);
    steam_sensor =
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
