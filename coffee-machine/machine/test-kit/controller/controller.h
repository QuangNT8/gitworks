#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <stdint.h>
#include "queue.h"
#include "stm32f0xx.h"
#define ACTION_QUEUE_SIZE   128

namespace testkit
{

class Controller
{
public:
    void init();
    void loop();
    void openValve(uint8_t index, uint16_t time);
    void delay(uint16_t time);

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

extern testkit::Controller CONTROLLER;
}

#endif // CONTROLLER_H
