#include "controller.h"
#include "motor/controller.h"
#include "valve/controller.h"

machine::Controller machine::CONTROLLER;

void machine::Controller::init()
{
    actionQueue_.init(actionBuf_,ACTION_QUEUE_SIZE);
    timeVal = 0;
}

void machine::Controller::loop()
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
                    case MOVE: moveTo_(); break;
                    case OPEN: openValve_(); break;
                    case DELAY: delay_(); break;
                }
            }
            break;
        }
        case MOVE:
        {
            if (!motor::CONTROLLER.moving()) action_ = NONE;
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

void machine::Controller::moveTo(int32_t d)
{
    if (actionQueue_.available()>5)
    {
        actionQueue_.push(MOVE);
        actionQueue_.push((d >> 24) & 0xFF);
        actionQueue_.push((d >> 16) & 0xFF);
        actionQueue_.push((d >> 8) & 0xFF);
        actionQueue_.push(d & 0xFF);
    }
}

void machine::Controller::moveTo_()
{
    int32_t d = actionQueue_.pop();
    d <<= 8;
    d += actionQueue_.pop();
    d <<= 8;
    d += actionQueue_.pop();
    d <<= 8;
    d += actionQueue_.pop();
    motor::CONTROLLER.setPosition(d);
}

void machine::Controller::openValve(uint8_t index, uint16_t time)
{
    if (actionQueue_.available()>4)
    {
        actionQueue_.push(OPEN);
        actionQueue_.push(index);
        actionQueue_.push((time >> 8) & 0xFF);
        actionQueue_.push(time & 0xFF);
    }
}

void machine::Controller::openValve_()
{
    uint8_t index = actionQueue_.pop();
    uint16_t time = actionQueue_.pop();
    time <<= 8;
    time += actionQueue_.pop();
    valve::CONTROLLER.open(index, time);
}

void machine::Controller::delay(uint16_t time)
{
    if (actionQueue_.available()>3)
    {
        actionQueue_.push(DELAY);
        actionQueue_.push((time >> 8) & 0xFF);
        actionQueue_.push(time & 0xFF);
    }
}

void machine::Controller::delay_()
{
    uint16_t time = actionQueue_.pop();
    time <<= 8;
    time += actionQueue_.pop();
    valve::CONTROLLER.delay(time);
}

void machine::Controller::option(uint8_t valve)
{
    openValve(valve, timeVal);
}
