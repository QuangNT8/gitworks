#ifndef VALVE_CONTROLLER_H
#define VALVE_CONTROLLER_H

#include "stm32f0xx.h"

namespace valve
{

class Controller
{
public:
    void init();
    void loop();
    bool openning(){return opening_;}
    bool delaying(){return delaying_;}
    void open(uint8_t index, uint16_t time);    //T = time*20ms; for ex: time = 5 ~ 100ms, time = 50 ~ 1s, time = 100 ~ 2s
    void delay(uint16_t time);                  //T = time*20ms; for ex: time = 5 ~ 100ms, time = 50 ~ 1s, time = 100 ~ 2s
    void updateSignals();
private:
    void moveLeft_(uint8_t index);
    void moveRight_(uint8_t index);
    void moveMiddle_(uint8_t index);

    enum State{HIGH=1, LEFT, MIDDLE, RIGHT};
    bool opening_ = false, delaying_ = false;
    uint16_t openTimeCount_ = 0, delayTimeCount_ = 0;
    uint8_t pwmState_ = HIGH;
    uint16_t leftBits_ = 0;
    uint16_t midleBits_ = 0;
    uint8_t currentIndex_ = 0;
    uint16_t openTime_ = 0, delayTime_ = 0;
};

extern valve::Controller CONTROLLER;

}

#endif // CONTROLLER_H

