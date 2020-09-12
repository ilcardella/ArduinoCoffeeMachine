#pragma once

#include "Common.h"

template <class Adapter> class BaseSerialInterface
{
  public:
    virtual ~BaseSerialInterface() = default;

    virtual void read_input() = 0;

    virtual void print_status(const Gaggia::ControlStatus<Adapter> &status) = 0;

    virtual bool is_debug_active() = 0;
    virtual double get_mock_temperature() = 0;

    virtual bool get_new_kp(double *kp) = 0;
    virtual bool get_new_ki(double *ki) = 0;
    virtual bool get_new_kd(double *kd) = 0;
};

template <class Adapter> class BaseDisplay
{
  public:
    virtual ~BaseDisplay() = default;

    virtual bool update(const Gaggia::ControlStatus<Adapter> &status) = 0;
};

class BaseHeater
{
  public:
    virtual ~BaseHeater() = default;

    virtual bool set(const bool &on) = 0;
};

class BaseModeDetector
{
  public:
    virtual ~BaseModeDetector() = default;

    virtual Gaggia::Mode get_mode() = 0;
};

class BasePIDController
{
  public:
    virtual ~BasePIDController() = default;

    virtual bool compute(const double &input, const double &setpoint, bool *relay_on) = 0;

    virtual void set_kp(const double &kp) = 0;
    virtual void set_ki(const double &ki) = 0;
    virtual void set_kd(const double &kd) = 0;
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

template <class Adapter> class BaseTemperatureSensor
{
  public:
    virtual ~BaseTemperatureSensor() = default;

    /** Return the name identifier of the sensor.
     */
    virtual typename Adapter::String get_name() = 0;

    /* Read the sensor and store the current temperature in
     * celsius degrees into 'value'.
     * Return 'true' if the operation succeeds, 'false' otherwise
     */
    virtual bool get_temperature_celsius(float *value) = 0;
};