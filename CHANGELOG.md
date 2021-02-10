# Gaggia PID Controller Changelog

## []
### Added
- Thermocouple sensor support a temperature offset to account for static error
- Wrapper for all Arduino.h functions and types
- CMake configuration for non-Arduino dependant code
- Automated test suite for non-Arduino dependant codebase
- Circuit diagram in documentation

### Changed
- Reformat of the `TemperatureSensor` class using template for sensor implementation
- Replaced use of Arduino String with char array
- Configuration for coffee_machine library is provided through a template argument
- Use `lib-coffee-machine` external library for core functionalities

### Fixed
- Resolved crash due to String assignments
- Fixed print formatting of float and double
- Fixed bug in MovingAverage affected by the creation of an instance with 0 history length

## [1.1.0] - 2020-08-22
### Added
- Added python script to plot machine temperatures reported through serial interface
- Configurable safety timeouts to turn the heater off after a set period of time
- Added support for K-type thermocouple sensors with MAX6675 and SPI interface

### Changed
- Display shows status of the machine indicating if it's heating, cooling or ready for brew
- Performed PID tuning to reduce oscillation of temperature
- Updated steam switch mode to use internal pullup resistor

## [1.0.0] - 2020-05-03
### Added
- First release:
  - Support for TSic306 digital temperature sensor
  - SSR control with PID loop
  - Support for SSD1306 OLED 128x64 display
  - Serial interface to show debug information
