#ifndef VALVE_CONTROLLER_H
#define VALVE_CONTROLLER_H

//#define LEFT_ARR    500 //1ms , FIXME:
//#define MIDDLE_ARR  250 //1.5ms, FIXME:
//#define RIGHT_ARR   250 //2ms, FIXME:
//#define HIGH_ARR    (10000 - LEFT_ARR - MIDDLE_ARR - RIGHT_ARR)

#define LEFT_ARR    250//new mr Hung
#define MIDDLE_ARR  420
#define RIGHT_ARR   500
#define HIGH_ARR    (10000 - LEFT_ARR - MIDDLE_ARR - RIGHT_ARR)

#define DELAY_TIME  30 //600ms
#define VALVE       15

#include "stm32f103xb.h"

namespace valve
{

class Controller
{
public:
    void init();
    void loop();
    void open(uint16_t time);    //T = time*20ms; for ex: time = 5 ~ 100ms, time = 50 ~ 1s, time = 100 ~ 2s
    void updateSignals();
private:
    void valveOpened_();/*TODO*/
    void valveClosed_();/*TODO*/

    void moveLeft_();
    void moveRight_();
    void moveMiddle_();

    enum Position{HIGH=1, LEFT, MIDDLE, RIGHT};
    enum State{CLOSE = 1, DELAY, OPEN};

    uint8_t pwmState_ = HIGH;
    uint16_t leftBits_ = 0;
    uint16_t midleBits_ = 0;
    uint16_t times_;
    uint16_t state_;
    bool needUpdateTime_ = false;
};

extern valve::Controller CONTROLLER;

}

#endif // CONTROLLER_H

