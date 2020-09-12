#include "libraries/ArduinoAdapter.h"
#include "libraries/Display.h"
#include "libraries/Factories.h"
#include "libraries/Heater.h"
#include "libraries/ModeDetector.h"
#include "libraries/RelayPIDController.h"
#include "libraries/SerialInterface.h"
#include "libraries/coffee_machine/BaseTypes.h"
#include "libraries/coffee_machine/CoffeeMachine.h"

using Adapter = ArduinoAdapter;

CoffeeMachine<Adapter> *machine;
BaseDisplay<Adapter> *display;
BaseSerialInterface<Adapter> *serial;
BasePIDController *pid;
BaseModeDetector *mode_detector;
BaseHeater *heater;
BaseTemperatureSensor<Adapter> *water_sensor;
BaseTemperatureSensor<Adapter> *steam_sensor;

void setup()
{
    display = new Display<Adapter>();
    serial = new SerialInterface<Adapter>(Configuration::SERIAL_BAUDRATE);
    pid = new RelayPIDController<Adapter>(Configuration::P_GAIN, Configuration::I_GAIN,
                                          Configuration::D_GAIN);
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
    machine->spin();
}
