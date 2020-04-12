/* GaggiaPIDController
 */

#include "RelayPIDController.h"
#include "TemperatureSensor.h"

/************************************************/
// USER SETTINGS
/************************************************/

// Input pin of the water temperature sensor
#define WATER_TEMP_PIN 4
// Input pin of the steam temperature sensor
#define STEAM_TEMP_PIN 5
// Output PWM pin to control the boiler
#define BOILER_SSR_PIN 6
// Input pin to detect steam mode
#define STEAM_SWITCH_PIN 7
// Target water temperature in celsius
#define TARGET_WATER_TEMP 93.5
// Target steam temperature in celsius
#define TARGET_STEAM_TEMP 146.0
// Enable serial comms for debugging
#define ENABLE_DEBUG_SERIAL 1
// PID gain parameters
#define P_GAIN 2
#define I_GAIN 5
#define D_GAIN 1

/************************************************/

// Global variables and structs
enum MODE
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

struct ControlStatus
{
    MODE machine_mode;
    double current_temperature;
    double target_temperature;
};

TemperatureSensor *water_sensor;
TemperatureSensor *steam_sensor;
RelayPIDController *pid;
ControlStatus machine_status;

void setup()
{
    // Initialise serial comm
    if (ENABLE_DEBUG_SERIAL)
    {
        Serial.begin(9600);
    }

    // Setup the pin modes
    pinMode(WATER_TEMP_PIN, INPUT);
    pinMode(STEAM_TEMP_PIN, INPUT);
    pinMode(BOILER_SSR_PIN, OUTPUT);
    pinMode(STEAM_SWITCH_PIN, INPUT);
    // Initialise sensors and other members
    water_sensor = new TemperatureSensor(WATER_TEMP_PIN, "water_sensor");
    steam_sensor = new TemperatureSensor(STEAM_TEMP_PIN, "steam_sensor");
    pid = new RelayPIDController(P_GAIN, I_GAIN, D_GAIN);
    machine_status.current_temperature = 0.0;
    machine_status.target_temperature = TARGET_WATER_TEMP;
    machine_status.machine_mode = MODE::WATER_MODE;

    // Initialse display
    // TODO
}

void loop()
{
    if (!update_machine_status(&machine_status))
    {
        // TODO signal error on display
        return;
    }

    // Compute PID controller step with current data
    uint8_t relay_request = pid->compute(&machine_status.current_temperature,
                                         &machine_status.target_temperature);

    // Set boiler SSR On or Off
    set_boiler_status(relay_request);

    // Update display
    // TODO

    // Debug serial prints
    message("Operation mode: " + String(machine_status.machine_mode));
    message("Current temp: " + String(machine_status.current_temperature));
    message("Target temp: " + String(machine_status.target_temperature));
    message("SSR status: " + String(relay_request));
}

bool update_machine_status(ControlStatus *status)
{
    // Read operation mode
    status->machine_mode = get_machine_mode();
    // Set target temperature based on machine mode
    status->target_temperature =
        (status->machine_mode == WATER_MODE) ? TARGET_WATER_TEMP : TARGET_STEAM_TEMP;

    // Select correct sensor for current operation mode
    TemperatureSensor *sensor =
        (status->machine_mode == WATER_MODE) ? water_sensor : steam_sensor;

    // Get the current temp from the temperature sensor
    float sensor_value;
    if (!sensor->get_temperature_celsius(&sensor_value))
    {
        // TODO add a String message in ControlStatus
        message("Unable to read temperature from sensor: " + sensor->get_name());
        return false;
    }
    status->current_temperature = static_cast<double>(sensor_value);
    return true;
}

/* Return the current working mode of the machine */
MODE get_machine_mode()
{
    int value = digitalRead(STEAM_SWITCH_PIN);
    return value ? MODE::STEAM_MODE : MODE::WATER_MODE;
}

void set_boiler_status(const uint8_t &value)
{
    pinMode(BOILER_SSR_PIN, value);
}

void message(const String &msg)
{
    if (ENABLE_DEBUG_SERIAL)
    {
        Serial.println(msg);
    }
}
