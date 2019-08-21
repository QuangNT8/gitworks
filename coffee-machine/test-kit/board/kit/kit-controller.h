#ifndef KIT_CONTROLLER_H
#define KIT_CONTROLLER_H

#include "queue.h"
#include "stm32f103xb.h"

namespace kit
{

class Controller
{
public:
    /*init System clock*/
    void initSystem();

    void init();
    void loop();

    /*Pump*/
    void openPump();
    void closePump();
    void togglePump();
private:
    /*init Pump*/
    void initGPIO_();

    void pumpOpened_();
    void pumpClosed_();
};

extern kit::Controller CONTROLLER;

}

#endif // KITCONTROLLER_H
