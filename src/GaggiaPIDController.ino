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
// PID gain parameters
#define P_GAIN 100
#define I_GAIN 50
#define D_GAIN 20

/************************************************/

// Global variables and structs
enum MODE
{
    WATER_MODE = 0,
    STEAM_MODE = 1
};

struct ControlStatus
{
    MODE machine_mode = MODE::WATER_MODE;
    double current_temperature = 0.0;
    double target_temperature = TARGET_WATER_TEMP;
    bool water_heater_on = false;
    String status_message = "Loading...";
};

struct SerialComms
{
    bool debug_mode = true;
    unsigned long last_sent = 0;
    uint16_t print_timeout = 1000;
};

TemperatureSensor *water_sensor;
TemperatureSensor *steam_sensor;
RelayPIDController *pid;
ControlStatus machine_status;
SerialComms serial_comms;

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

    Serial.begin(9600);

    initialise_display();
}

void loop()
{
    read_serial();

    update_machine_status(&machine_status);

    pid->compute(&machine_status.current_temperature, &machine_status.target_temperature,
                 &machine_status.water_heater_on);

    set_heater_status(machine_status.water_heater_on);

    update_display();

    print_status_on_serial();
}

bool update_machine_status(ControlStatus *status)
{
    status->status_message = "";
    // Read operation mode
    status->machine_mode = get_machine_mode();
    // Set target temperature based on machine mode
    status->target_temperature =
        (status->machine_mode == WATER_MODE) ? TARGET_WATER_TEMP : TARGET_STEAM_TEMP;

    // When debug mode is enabled do not read sensors
    if (serial_comms.debug_mode)
        return true;

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

void read_serial()
{
    // Read input data and enable or disable the debug mode
    // If debug mode is enabled, accept temperature input
    // to overwrite sensor readings
    if (Serial.available())
    {
        auto input = Serial.readStringUntil('\n');
        if (input.startsWith("debug on"))
        {
            serial_comms.debug_mode = true;
        }
        else if (input.startsWith("debug off"))
        {
            serial_comms.debug_mode = false;
        }
        else if (serial_comms.debug_mode)
        {
            machine_status.current_temperature = input.toDouble();
        }
    }
}

void print_status_on_serial()
{
    auto now = millis();
    if (serial_comms.debug_mode &&
        now - serial_comms.last_sent > serial_comms.print_timeout)
    {
        serial_comms.last_sent = now;
        Serial.println("Operation mode: " + String(machine_status.machine_mode));
        Serial.println("Current temp: " + String(machine_status.current_temperature));
        Serial.println("Target temp: " + String(machine_status.target_temperature));
        Serial.println("SSR status: " + String(machine_status.water_heater_on));
        Serial.println("Message: " + machine_status.status_message);
    }
}
