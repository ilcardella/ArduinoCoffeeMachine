.. GaggiaPIDController documentation master file, created by
   sphinx-quickstart on Sat May  9 19:13:01 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

GaggiaPIDController's documentation
###################################

.. toctree::
   :maxdepth: 1
   :numbered:

Introduction
************

``GaggiaPIDController`` is a modification for Gaggia coffee machines replacing the
existing thermostats with a PID controller powered by Arduino.
I am confident that this modification could be applied to other machines too, but
in this documentation I will describe how I applied this modification to a ``Gaggia Paros``.
The``Gaggia Paros`` is almost identical in its parts to a ``Gaggia Classic`` and it comes
combined with a coffee grinder.

To add a PID controller to the coffee machine we'll need to replace the existing
thermostats that control the heater. There are 2 thermostats to allow the operation
of the machine heating water to brew temperature and also steam temperature.
Unfortunately these thermostat have a wide working range, meaning that they don't keep
the water/steam temperature stable enough to allow consistency of the brewed coffee.

Overview
********

This is the electric diagram of the ``Gaggia Paros``:

.. figure:: schematics/gaggia_paros_wiring.png
    :align: center
    :alt: Gaggia Paros electrical wiring

    Gaggia Paros electrical wiring

The switch ``5`` and ``6`` represents the two thermostats that are going to be replaced
with two digital temperature sensors and a Solid State Relay that the Arduino MCU will
use to directly control the heater.
The second modification required to allow to target the correct temperature is to
connect Arduino to the left pins of the "double switch" ``13``. This will allow the MCU
to know if the Steam button has been pressed.

Temperature sensors
*******************

TBD

Solid State Relay
*****************

TBD

Power supply
************

TBD

Display
*******

TBD

Assembly
********

TBD
