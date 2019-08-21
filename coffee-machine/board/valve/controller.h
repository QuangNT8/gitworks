#ifndef VALVE_CONTROLLER_H
#define VALVE_CONTROLLER_H

#define LEFT_ARR    250//new mr Hung
#define MIDDLE_ARR  420
#define RIGHT_ARR   500
#define HIGH_ARR    (10000 - LEFT_ARR - MIDDLE_ARR - RIGHT_ARR)

#define ALL_VALVES  0xFFFFU
#define MIN_VALVE_INDEX  0
#define MAX_VALVE_INDEX  15
#define DELAY_TIME  30 //600ms

#include "stm32f0xx.h"

namespace valve
{

class Controller
{
public:
    void init();
    void loop();
    void open(uint16_t index, uint16_t time);    //T = time*20ms; for ex: time = 5 ~ 100ms, time = 50 ~ 1s, time = 100 ~ 2s
    void updateSignals();
private:
    void valveOpened_(uint16_t index);
    void valveClosed_(uint16_t index);

    void moveLeft_(uint16_t index);
    void moveRight_(uint16_t index);
    void moveMiddle_(uint16_t index);

    enum Position{HIGH=1, LEFT, MIDDLE, RIGHT};
    enum State{CLOSE = 1, DELAY, OPEN};

    uint8_t pwmState_ = HIGH;
    uint16_t leftBits_ = 0;
    uint16_t midleBits_ = 0;
    uint16_t times_[MAX_VALVE_INDEX];
    uint16_t state_[MAX_VALVE_INDEX];
    bool needUpdateTime_ = false;
};

extern valve::Controller CONTROLLER;

}

#endif // CONTROLLER_H

