GaggiaPIDController's documentation
###################################

.. important:: This project is a work in progress, I will update the documentation while
   I progress with the work and things might change along the road

Introduction
************

``GaggiaPIDController`` is a modification for Gaggia coffee machines replacing the
existing thermostats with a PID controller powered by Arduino.
This modification could be applied to other machines too, but
in this document I will describe how I modified a ``Gaggia Paros``.
The ``Gaggia Paros`` is almost identical in its parts to a ``Gaggia Classic`` and it comes
combined with a coffee grinder.

To add a PID controller to the coffee machine we'll need to replace the existing
thermostats that control the heater. There are 2 thermostats to allow the operation
of the machine heating water to brew temperature and also steam temperature.
Unfortunately these thermostat do not allow fine control of the water temperature,
meaning that they don't keep the water/steam temperature stable enough to allow
consistency of the brewed coffee.

.. toctree::
   :maxdepth: 1
   :numbered:
   :caption: Documentation

   src/documentation
