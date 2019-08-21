#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "queue.h"
#include "stm32f103xb.h"
#include "command.h"
#include "queue.h"
#define ACTION_QUEUE_SIZE   128

namespace kit
{

class Controller
{
public:

    Controller();

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

    void openValve_();
    void delay_();

    enum State{NONE = 0, OPEN, DELAY};
    uint16_t action_ = NONE;

    /* action queue */
    Queue actionQueue_;
    uint8_t actionBuf_[ACTION_QUEUE_SIZE];

};

extern kit::Controller CONTROLLER;

}

#endif // CONTROLLER_H

