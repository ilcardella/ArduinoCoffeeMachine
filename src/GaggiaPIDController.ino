/* GaggiaPIDController
 */

#include "Display.h"
#include "RelayPIDController.h"
#include "SerialInterface.h"
#include "TemperatureSensor.h"
#include "libraries/Common.h"

/************************************************/
// USER SETTINGS
/************************************************/

// Input pin of the water temperature sensor
#define WATER_TEMP_PIN 2
// Input pin of the steam temperature sensor
#define STEAM_TEMP_PIN 3
// Output PWM pin to control the boiler
#define HEATER_SSR_PIN 4
// Input pin to detect steam mode
#define STEAM_SWITCH_PIN 5
// Target water temperature in celsius
#define TARGET_WATER_TEMP 93.5
// Target steam temperature in celsius
#define TARGET_STEAM_TEMP 146.0
// PID gain parameters
#define P_GAIN 100
#define I_GAIN 50
#define D_GAIN 20
#define SERIAL_BAUDRATE 9600

/************************************************/

// Global variables and structs
TemperatureSensor *water_sensor;
TemperatureSensor *steam_sensor;
RelayPIDController *pid;
Gaggia::ControlStatus machine_status;
SerialInterface *serial;
Display *display;

void setup()
{
    // Setup the pin modes
    pinMode(WATER_TEMP_PIN, INPUT);
    pinMode(STEAM_TEMP_PIN, INPUT);
    pinMode(HEATER_SSR_PIN, OUTPUT);
    pinMode(STEAM_SWITCH_PIN, INPUT);

    // Initialise sensors and actuators
    water_sensor = new TemperatureSensor(WATER_TEMP_PIN, "water_sensor");
    steam_sensor = new TemperatureSensor(STEAM_TEMP_PIN, "steam_sensor");
    pid = new RelayPIDController(P_GAIN, I_GAIN, D_GAIN);
    serial = new SerialInterface(SERIAL_BAUDRATE);
    display = new Display();
}

void loop()
{
    serial->read_input();

    update_machine_status(&machine_status);

    pid->compute(&machine_status.current_temperature, &machine_status.target_temperature,
                 &machine_status.water_heater_on);

    set_heater_status(&machine_status.water_heater_on);

    serial->print_status(&machine_status);

    display->update(&machine_status);
}

bool update_machine_status(Gaggia::ControlStatus *status)
{
    status->status_message = "";
    // Read operation mode
    status->machine_mode = get_machine_mode();
    // Set target temperature based on machine mode
    status->target_temperature = (status->machine_mode == Gaggia::WATER_MODE)
                                     ? TARGET_WATER_TEMP
                                     : TARGET_STEAM_TEMP;

    // When debug mode is enabled do not read sensors
    if (serial->is_debug_active())
    {
        status->current_temperature = serial->get_mock_temperature();
        return true;
    }

    // Select correct sensor for current operation mode
    TemperatureSensor *sensor =
        (status->machine_mode == Gaggia::WATER_MODE) ? water_sensor : steam_sensor;

    // Get the current temp from the temperature sensor
    float sensor_value;
    if (!sensor->get_temperature_celsius(&sensor_value))
    {
        // TODO add a String message in ControlStatus
        status->status_message =
            "Unable to read temperature from sensor: " + sensor->get_name();
        return false;
    }

    status->current_temperature = static_cast<double>(sensor_value);

    return true;
}

/* Return the current working mode of the machine */
Gaggia::MODE get_machine_mode()
{
    int value = digitalRead(STEAM_SWITCH_PIN);
    return value ? Gaggia::STEAM_MODE : Gaggia::WATER_MODE;
}

void set_heater_status(const bool *heater_on)
{
    uint8_t pin_level = *heater_on ? HIGH : LOW;
    pinMode(HEATER_SSR_PIN, pin_level);
}
