# Gaggia PID Controller [![Documentation Status](https://readthedocs.org/projects/gaggiapidcontroller/badge/?version=latest)](https://gaggiapidcontroller.readthedocs.io/en/latest/?badge=latest) [![Build Status](https://travis-ci.com/ilcardella/GaggiaPIDController.svg?branch=master)](https://travis-ci.com/ilcardella/GaggiaPIDController)

This project is a PID controller for a `Gaggia Paros` (or Gaggia Classic) coffee machine using Arduino and a few other components.
This Arduino based controller improves the stability of the water temperature of the coffee machine by controlling the boiler with a PID control loop feedback.

## Software

The codebase is mainly C++ and it's designed to isolate the Arduino specific parts in
implementation details classes.
Most of the components are template classes that accept an `Adapter` type as parameter.
This `Adapter` class allows to abstract the Arduino specific features requiring Arduino
specific headers and libraries.
The goal and the benefit of this is to allow the creation of a C++ library of
the non-Arduino dependant code adding automated unit testing in CI.

### Arduino libraries

This project depends on the following Arduino libraries:
- [PID](https://github.com/br3ttb/Arduino-PID-Library)
- [TSIC](https://github.com/Schm1tz1/arduino-tsic)
- [SDD1306Ascii](https://github.com/greiman/SSD1306Ascii)
- [MAX6675](https://github.com/adafruit/MAX6675-library)

### Configuration

The code configuration is contained in a "static" `struct` in the file `Configuration.h`.
To change the default configuration just edit the default value of the desired parameter.

### Makefile

The `Makefile` at the project root is the entrypoint for development actions such as
build, test and build the docs.

### Build

You can build the code with different approaches:
- with the Arduino IDE opening the `GaggiaPIDController.ino` sketch
- with VSCode and the [Arduino extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino) opening the `GaggiaPIDController.ino` sketch
- with `make build` using Docker

#### Build with Docker

The code can be built inside a Docker container using docker-compose. The `docker-compose.yml` and the `Dockerfile` are in the `docker` directory. The `docker-compose.yml` depends on 2 environment variables that must be defined before starting the build:
- CORE: the Arduino core
- BOARD: the Arduino board to use

Refer to the [arduino-cli](https://arduino.github.io/arduino-cli/) documentation for the list of available cores and boards.
The following is an example that builds the code for an Arduino Nano board:

```
$ cd /path/to/repo
$ CORE=avr BOARD=nano make build
```

The generated build files will be in the `GaggiaPIDController/build` directory.

After building the Docker image the first time, you can then use the `arduino-cli` installed in the Docker image directly with:

```
$ cd /path/to/repo
$ docker run --rm -it -v $PWD:/build arduino-builder arduino-cli version
arduino-cli Version: 0.10.0 Commit: ec5c3ed
```

## Hardware

### Components

This is the list of components I used for the project:
- 1x Arduino Nano (any board type will do)
- 1x Solid State Relay 40A
- 2x Digital temperature sensors (e.g. TSic306)
- 1x I2C 128x64 OLED
- Wires and other supplies

## Documentation

Read the project documentation at: https://gaggiapidcontroller.readthedocs.io

Or build it locally with:

```
$ cd /path/to/repo
$ make docs
```

## Tools

The `tools` directory contains a Python script that can be used to read the Arduino
Serial messages and to plot the current water temperature on a graph. The goal
is to help the PID tuning process showing the realtime results.

```
$ cd tools
$ poetry install --no-dev
$ poetry run python temp_plotter.py
```

## Acknowledgement

A big thanks goes to these two other projects that helped me a lot in the understanding of the electric schematics of the coffee machine and the components to use:
- [RaspberryPI based](http://int03.co.uk/blog/project-coffee-espiresso-machine/)
- [Arduino based](http://www.cyberelectronics.org/?p=458)
