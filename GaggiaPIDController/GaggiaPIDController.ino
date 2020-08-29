/* GaggiaPIDController
 */

#include "Display.h"
#include "KTypeThermocouple.h"
#include "ModeDetector.h"
#include "RelayPIDController.h"
#include "SerialInterface.h"
#include "TSICTempSensor.h"
#include "TemperatureSensor.h"
#include "libraries/Common.h"

/************************************************/
// USER SETTINGS
/************************************************/

// Define the sensor type to use for the water temperature
#define WATER_TEMP_SENSOR_TYPE sensors::temperature::type::KTYPE_SPI
// Define the sensor type to use for the steam temperature
#define STEAM_TEMP_SENSOR_TYPE sensors::temperature::type::KTYPE_SPI

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
#define TARGET_STEAM_TEMP 150.0

// PID gain parameters
#define P_GAIN 125
#define I_GAIN 0.8
#define D_GAIN 0.75

// SPI interface common pins
#define SPI_CLK_PIN 8
#define SPI_DO_PIN 9

// Serial interface
#define SERIAL_BAUDRATE 9600

// Safety timeouts in milliseconds to turn off the heater. (disabled if < 1)
#define SAFETY_TIMEOUT 40 * 60 * 1000 // 40 minutes
#define STEAM_TIMEOUT 5 * 60 * 1000   // 5 minutes

/************************************************/

using namespace sensors::temperature;

// Global variables and structs
TemperatureSensor *water_sensor;
TemperatureSensor *steam_sensor;
RelayPIDController pid(P_GAIN, I_GAIN, D_GAIN);
SerialInterface serial(SERIAL_BAUDRATE);
Display display;
ModeDetector mode_detector(STEAM_SWITCH_PIN);
Gaggia::ControlStatus machine_status;

void setup()
{
    // Setup the pin modes. Other pins are initialised within classes
    pinMode(HEATER_SSR_PIN, OUTPUT);

    // Initialise the temperature sensors based on configuration
    water_sensor =
        make_temp_sensor(WATER_TEMP_SENSOR_TYPE, "water_sensor", WATER_TEMP_PIN);
    steam_sensor =
        make_temp_sensor(STEAM_TEMP_SENSOR_TYPE, "steam_sensor", STEAM_TEMP_PIN);

    // Mark machine start time
    machine_status.time_since_start = millis();

    // Allow sensors to initialise
    delay(500);
}

void loop()
{
    serial.read_input();

    if (update_machine_status(machine_status))
    {
        update_pid(machine_status);
    }

    set_heater_status(machine_status.water_heater_on);

    display.update(machine_status);

    serial.print_status(machine_status);
}

bool update_machine_status(Gaggia::ControlStatus &status)
{
    unsigned long now = millis();

    // Set SSR off by default and let the PID decide whether to turn it on
    status.water_heater_on = false;
    // Read operation mode
    status.machine_mode = mode_detector.get_mode();

    // Reset steam mode timeout counter when not in steam mode
    if (status.machine_mode != Gaggia::STEAM_MODE)
    {
        status.time_since_steam_mode = now;
    }

    // Set target temperature based on machine mode
    status.target_temperature = (status.machine_mode == Gaggia::WATER_MODE)
                                    ? TARGET_WATER_TEMP
                                    : TARGET_STEAM_TEMP;

    // When debug mode is enabled do not read sensors
    if (serial.is_debug_active())
    {
        status.current_temperature = serial.get_mock_temperature();
        status.status_message = "Debug mode";
        return true;
    }

    // Select correct sensor for current operation mode
    TemperatureSensor *sensor =
        (status.machine_mode == Gaggia::WATER_MODE) ? water_sensor : steam_sensor;

    // Get the current temp from the temperature sensor
    float sensor_value;
    if (not sensor || not sensor->get_temperature_celsius(&sensor_value))
    {
        status.status_message =
            "Unable to read temperature from sensor: " + sensor->get_name();
        return false;
    }

    status.current_temperature = static_cast<double>(sensor_value);

    // Use a tolerance of +/- 1Deg for the message
    double diff = status.target_temperature - status.current_temperature;
    if (diff < -1.0)
    {
        status.status_message = "Cooling...";
    }
    else if (diff > 1.0)
    {
        status.status_message = "Heating...";
    }
    else
    {
        status.status_message = "Ready";
    }

    // If the machine has been on for more than the safety limit, then report a problem
    // so the heater will be turned off
    if (SAFETY_TIMEOUT > 0 && (now - status.time_since_start) > SAFETY_TIMEOUT)
    {
        status.status_message = "Safety timeout expired";
        return false;
    }
    // Check steam mode timeout to avoid keeping the machine at high temps for long
    if (STEAM_TIMEOUT > 0 && (now - status.time_since_steam_mode) > STEAM_TIMEOUT)
    {
        status.status_message = "Steam mode timeout expired";
        return false;
    }

    return true;
}

void update_pid(Gaggia::ControlStatus &status)
{
    // Check if new PID gains have been requested and update our controller
    double gain;
    if (serial.get_new_kp(&gain))
    {
        pid.set_kp(gain);
    }
    if (serial.get_new_ki(&gain))
    {
        pid.set_ki(gain);
    }
    if (serial.get_new_kd(&gain))
    {
        pid.set_kd(gain);
    }

    pid.compute(status.current_temperature, status.target_temperature,
                &(status.water_heater_on));
}

void set_heater_status(const bool &heater_on)
{
    digitalWrite(HEATER_SSR_PIN, heater_on ? HIGH : LOW);
}

TemperatureSensor *make_temp_sensor(const type &sensor_type, const String &name,
                                    const uint8_t &sensor_pin)
{
    switch (sensor_type)
    {
    case type::TSIC:
        return new TSICTempSensor(name, sensor_pin);
        break;
    case type::KTYPE_SPI:
        return new KTypeThermocouple(name, SPI_CLK_PIN, SPI_DO_PIN, sensor_pin);
        break;
    default:
        // Ideally we would raise an exception here
        return nullptr;
        break;
    }
}
