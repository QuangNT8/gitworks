#include "controller.h"
#include "hal/gpio.h"
#include "command.h"

blink::Controller blink::CONTROLLER;

void blink::Controller::init()
{
    hal::GPIO::configPinOutput(17);
    hal::GPIO::configPinOutput(18);
    hal::GPIO::configPinOutput(19);
    hal::GPIO::configPinOutput(20);

    /*set threshold*/
    plot::CONTROLLER.set_current_threshold(215);
}

uint32_t counterLed = 0;
bool onOffbool = true;
void blink::Controller::loop()
{
    if(counterLed++ > (1 << 18))
    {
        onOffbool = !onOffbool;
        if(onOffbool)
        {
            ledOn_();
        }
        else
        {
            ledOff_();
        }
        counterLed = 0;
    }
}


void blink::Controller::ledOn_()
{
    hal::GPIO::pinClear(17);
    hal::GPIO::pinClear(18);
    hal::GPIO::pinClear(19);
    hal::GPIO::pinClear(20);
}

void blink::Controller::ledOff_()
{
    hal::GPIO::pinSet(17);
    hal::GPIO::pinSet(18);
    hal::GPIO::pinSet(19);
    hal::GPIO::pinSet(20);
}

