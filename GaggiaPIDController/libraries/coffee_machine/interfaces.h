#pragma once

class BaseSerialInterface
{
  public:
    virtual ~BaseSerialInterface() = default;

    virtual void begin(const unsigned long &baudrate) = 0;
    virtual int available() = 0;
    virtual void read_string_until(const char &terminator, char *data) = 0;
    virtual size_t println(const char *value) = 0;
};

class BaseDisplay
{
  public:
    virtual ~BaseDisplay() = default;

    virtual bool initialise() = 0;
    virtual bool clear() = 0;
    virtual bool print(const unsigned &col, const unsigned &row, const char *data) = 0;
    virtual bool print(const unsigned &col, const unsigned &row, const int &data) = 0;
    virtual bool print(const unsigned &col, const unsigned &row, const float &data) = 0;
    virtual bool print(const unsigned &col, const unsigned &row, const double &data) = 0;
};

class IOPin
{
  public:
    enum class Modes
    {
        IN,
        IN_PU,
        OUT
    };

    virtual ~IOPin() = default;

    virtual void set_mode(const Modes &mode) = 0;
    virtual bool is_high() = 0;
    virtual bool is_low() = 0;
    virtual void digital_write_high() = 0;
    virtual void digital_write_low() = 0;
};

class Controller
{
  public:
    virtual ~Controller() = default;

    virtual void set_output_limits(const double &min, const double &max) = 0;
    virtual bool compute(const double &input, const double &setpoint, double &output) = 0;
    virtual bool update_settings(char *raw) = 0;
};

class BaseSensor
{
  public:
    virtual ~BaseSensor() = default;

    /* Read the sensor and store the output into 'value'.
     * Return 'true' if the operation succeeds, 'false' otherwise
     */
    virtual bool read_sensor(float *value) = 0;
};

class BaseTemperatureSensor
{
  public:
    virtual ~BaseTemperatureSensor() = default;

    /** Return the name identifier of the sensor.
     */
    virtual char *get_name() = 0;

    /* Read the sensor and store the current temperature in
     * celsius degrees into 'value'.
     * Return 'true' if the operation succeeds, 'false' otherwise
     */
    virtual bool get_temperature_celsius(float *value) = 0;
};