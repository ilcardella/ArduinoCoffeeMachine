#include "libraries/ArduinoAdapter.h"
#include "libraries/CoffeeMachine.h"

using Adapter = ArduinoAdapter;

CoffeeMachine<Adapter> *machine;

void setup()
{
    machine = new CoffeeMachine<Adapter>();

    // Allow sensors to initialise
    Adapter::delay(500);
}

void loop()
{
    machine->spin();
}
