#ifndef MACHINE_CONTROLLER_H
#define MACHINE_CONTROLLER_H
#include <stdint.h>
#include "queue.h"
#define ACTION_QUEUE_SIZE   128

namespace machine
{

class Controller
{
public:
    void init();
    void loop();
    void moveTo(int32_t d);
    void openValve(uint8_t index, uint16_t time);
    void delay(uint16_t time);

    void option(uint8_t valve);
    uint16_t timeVal;

private:
    void moveTo_();
    void openValve_();
    void delay_();

    enum State{NONE = 0, MOVE, OPEN, DELAY};
    uint16_t action_ = NONE;

    /* action queue */
    Queue actionQueue_;
    uint8_t actionBuf_[ACTION_QUEUE_SIZE];
};

extern machine::Controller CONTROLLER;
}

#endif // CONTROLLER_H
