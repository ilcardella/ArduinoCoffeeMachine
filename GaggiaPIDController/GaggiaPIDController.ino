/* GaggiaPIDController
 */

#include "Display.h"
#include "ModeDetector.h"
#include "RelayPIDController.h"
#include "SerialInterface.h"
#include "TemperatureSensor.h"
#include "libraries/Common.h"

/************************************************/
// USER SETTINGS
/************************************************/

// Input pin of the water temperature sensor
#define WATER_TEMP_PIN 4
// Input pin of the steam temperature sensor
#define STEAM_TEMP_PIN 5
// Output PWM pin to control the boiler
#define HEATER_SSR_PIN 6
// Input pin to detect steam mode
#define STEAM_SWITCH_PIN 7
// Target water temperature in celsius
#define TARGET_WATER_TEMP 95.0
// Target steam temperature in celsius
#define TARGET_STEAM_TEMP 145.0
// PID gain parameters
#define P_GAIN 250
#define I_GAIN 1.5
#define D_GAIN 0.75
#define SERIAL_BAUDRATE 9600

/************************************************/

// Global variables and structs
TemperatureSensor *water_sensor;
TemperatureSensor *steam_sensor;
RelayPIDController *pid;
SerialInterface *serial;
Display *display;
ModeDetector *mode_detector;

void setup()
{
    // Setup the pin modes
    pinMode(WATER_TEMP_PIN, INPUT);
    pinMode(STEAM_TEMP_PIN, INPUT);
    pinMode(HEATER_SSR_PIN, OUTPUT);
    pinMode(STEAM_SWITCH_PIN, INPUT_PULLUP);

    // Initialise sensors and actuators
    water_sensor = new TemperatureSensor(WATER_TEMP_PIN, "water_sensor");
    steam_sensor = new TemperatureSensor(STEAM_TEMP_PIN, "steam_sensor");
    pid = new RelayPIDController(P_GAIN, I_GAIN, D_GAIN);
    serial = new SerialInterface(SERIAL_BAUDRATE);
    display = new Display();
    mode_detector = new ModeDetector(STEAM_SWITCH_PIN);
}

void loop()
{
    Gaggia::ControlStatus machine_status;

    serial->read_input();

    if (update_machine_status(&machine_status))
    {
        update_pid(&machine_status);
    }

    set_heater_status(&machine_status.water_heater_on);

    display->update(&machine_status);

    serial->print_status(&machine_status);
}

bool update_machine_status(Gaggia::ControlStatus *status)
{
    // Set SSR off by default and let the PID decide whether to turn it on
    status->water_heater_on = false;
    // Read operation mode
    status->machine_mode = mode_detector->get_mode();
    // Set target temperature based on machine mode
    status->target_temperature = (status->machine_mode == Gaggia::WATER_MODE)
                                     ? TARGET_WATER_TEMP
                                     : TARGET_STEAM_TEMP;

    // When debug mode is enabled do not read sensors
    if (serial->is_debug_active())
    {
        status->current_temperature = serial->get_mock_temperature();
        status->status_message = "Debug mode";
        return true;
    }

    // Select correct sensor for current operation mode
    TemperatureSensor *sensor =
        (status->machine_mode == Gaggia::WATER_MODE) ? water_sensor : steam_sensor;

    // Get the current temp from the temperature sensor
    float sensor_value;
    if (not sensor->get_temperature_celsius(&sensor_value))
    {
        status->status_message =
            "Unable to read temperature from sensor: " + sensor->get_name();
        return false;
    }

    status->current_temperature = static_cast<double>(sensor_value);

    // Use a tolerance of +/- 1Deg for the message
    double diff = status->target_temperature - status->current_temperature;
    if (diff < -1.0)
    {
        status->status_message = "Cooling...";
    }
    else if (diff > 1.0)
    {
        status->status_message = "Heating...";
    }
    else
    {
        status->status_message = "Ready";
    }

    return true;
}

void update_pid(Gaggia::ControlStatus *status)
{
    // Check if new PID gains have been requested and update our controller
    uint16_t gain;
    if (serial->get_new_kp(&gain))
    {
        pid->set_kp(&gain);
    }
    if (serial->get_new_ki(&gain))
    {
        pid->set_ki(&gain);
    }
    if (serial->get_new_kd(&gain))
    {
        pid->set_kd(&gain);
    }

    pid->compute(&(status->current_temperature), &(status->target_temperature),
                 &(status->water_heater_on));
}

void set_heater_status(const bool *heater_on)
{
    digitalWrite(HEATER_SSR_PIN, (*heater_on) ? HIGH : LOW);
}
