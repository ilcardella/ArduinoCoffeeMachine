# Gaggia PID Controller

This project is a PID controller for a `Gaggia Paros` (or Gaggia Classic) coffee machine using Arduino and a few other components.
This Arduino based controller improves the stability of the water temperature of the coffee machine by controlling the boiler with a PID control loop feedback.

## Software

### Arduino libraries

This project depends on the following Arduino libraries:
- [PID](https://github.com/br3ttb/Arduino-PID-Library)
- [TSIC](https://github.com/Schm1tz1/arduino-tsic)
- [Adafruit_SDD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)

### Build

You can build the code using the Arduino IDE or with VSCode with the [Arduino extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino) opening the `GaggiaPIDController.ino` sketch.

#### Build with Docker

The code can be built inside a Docker container using a Docker image generated from the `Dockerfile` provided in the `docker` directory.
First build the Docker image:

```
$ cd /path/to/repo
$ docker build --rm -f docker/Dockerfile -t arduino-build .
```

The previous command will create a Docker image called `arduino-build` that can be used to build the code.
The following command generate the `.hex` for an Arduino Nano board:

```
$ docker run --rm -it -v /path/to/repo:/build arduino-build scripts/build-project.sh avr nano
```

## Hardware

### Components

This is the list of components required for this project:
- 1x Arduino (any board type will do)
- 1x Solid State Relay 40A
- 2x Digital temperature sensor TSic306
- 1x I2C OLED
- Wires and other utilities

### Schematics

The following is the original schematic of the coffee machine:

![gaggia_classic_circuit](docs/schematics/gaggia_classic_circuit.png "Gaggia Classic circuit")

This in how it looks with the Arduino controller and the sensors:

TBD

## Credits

A big thank you goes to these two other projects that helped me a lot in the understanding of the electric schematics of the coffee machine and the components to use:
- [RaspberryPI based](http://int03.co.uk/blog/project-coffee-espiresso-machine/)
- [Arduino based](http://www.cyberelectronics.org/?p=458)
