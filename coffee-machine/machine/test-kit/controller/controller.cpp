#include "controller.h"
#include "valve/controller.h"

testkit::Controller testkit::CONTROLLER;

void testkit::Controller::init()
{
    initGPIO_();
    actionQueue_.init(actionBuf_,ACTION_QUEUE_SIZE);
}

void testkit::Controller::loop()
{
    switch (action_)
    {
        case NONE:
        {
            if (actionQueue_.notEmpty())
            {
                action_ = actionQueue_.pop();
                switch (action_)
                {
                    case OPEN: openValve_(); break;
                    case DELAY: delay_(); break;
                }
            }
            break;
        }
        case OPEN:
        {
            if (!valve::CONTROLLER.openning()) action_ = NONE;
            break;
        }
        case DELAY:
        {
            if (!valve::CONTROLLER.delaying()) action_ = NONE;
            break;
        }
    }
}

void testkit::Controller::openValve(uint8_t index, uint16_t time)
{
    if (actionQueue_.available()>4)
    {
        actionQueue_.push(OPEN);
        actionQueue_.push(index);
        actionQueue_.push((time >> 8) & 0xFF);
        actionQueue_.push(time & 0xFF);
    }
}

void testkit::Controller::openValve_()
{
    uint8_t index = actionQueue_.pop();
    uint16_t time = actionQueue_.pop();
    time <<= 8;
    time += actionQueue_.pop();
    valve::CONTROLLER.open(index, time);
}

void testkit::Controller::delay(uint16_t time)
{
    if (actionQueue_.available()>3)
    {
        actionQueue_.push(DELAY);
        actionQueue_.push((time >> 8) & 0xFF);
        actionQueue_.push(time & 0xFF);
    }
}

void testkit::Controller::delay_()
{
    uint16_t time = actionQueue_.pop();
    time <<= 8;
    time += actionQueue_.pop();
    valve::CONTROLLER.delay(time);
}
