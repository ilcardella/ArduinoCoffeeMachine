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
#define HEATER_SSR_PIN 6
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
    bool water_heater_on;
    String status_message;

    ControlStatus()
    {
        current_temperature = 0.0;
        target_temperature = TARGET_WATER_TEMP;
        machine_mode = MODE::WATER_MODE;
        water_heater_on = false;
        status_message = "Loading...";
    }
};

TemperatureSensor *water_sensor;
TemperatureSensor *steam_sensor;
RelayPIDController *pid;
ControlStatus machine_status;

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

    initialiase_serial();

    initialise_display();
}

void loop()
{
    if (not update_machine_status(&machine_status))
    {
        // TODO signal error on display
        return;
    }

    // Compute PID controller step with current data
    if (not pid->compute(&machine_status.current_temperature,
                         &machine_status.target_temperature,
                         &machine_status.water_heater_on))
    {
        // TODO signal error on display
        return;
    }

    // Set heater SSR On or Off
    set_heater_status(machine_status.water_heater_on);

    update_display();

    print_status_on_serial();
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
        status->status_message =
            "Unable to read temperature from sensor: " + sensor->get_name();
        return false;
    }
    else
    {
        status->status_message = "";
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

void set_heater_status(const bool &heater_on)
{
    uint8_t pin_level = heater_on ? HIGH : LOW;
    pinMode(HEATER_SSR_PIN, pin_level);
}

//##########################
//# Display
//##########################

void initialise_display()
{
    // TODO
    ;
}

void update_display()
{
    // TODO
    ;
}

//##########################
//# Serial
//##########################

void initialiase_serial()
{
    if (ENABLE_DEBUG_SERIAL)
    {
        Serial.begin(9600);
    }
}

void print_status_on_serial()
{
    if (ENABLE_DEBUG_SERIAL)
    {
        Serial.println("Operation mode: " + String(machine_status.machine_mode));
        Serial.println("Current temp: " + String(machine_status.current_temperature));
        Serial.println("Target temp: " + String(machine_status.target_temperature));
        Serial.println("SSR status: " + String(machine_status.water_heater_on));
        Serial.println("Message: " + machine_status.status_message);
    }
}
