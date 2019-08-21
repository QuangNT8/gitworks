#include "scheduler.h"

bj::Scheduler::Result bj::Scheduler::Robot::createPlan(const std::vector<std::pair<uint64_t, double>>& ingredients)
{
    if (mode != FREE) return BUSY;
    uint16_t count = ingredients.size();
    for (auto& it: ingredients)
    {
        for (int i=0;i<STOP_COUNT; i++)
        {
            auto& action = actions[i];
            if (action.back->reserve(it.first, it.second))
            {
                action.vBack = it.second;
                count--;
            }
            else if (action.front->reserve(it.first, it.second))
            {
                action.vFront = it.second;
                count--;
            }
        }
    }
    if (count == 0)
    {
        if (other->mode == FREE) switchToBackward();
        else mode = FORWARD;
        this->moveNext();
        return OK;
    }
    this->reset();
    return OUT_OF_LIQUID;
}

//#define SAFE_SLOT_COUNT 1
#define SAFE_SLOT_COUNT 0
bool bj::Scheduler::Robot::isUnlocked(int16_t index)
{
    if (other->mode != BACKWARD) return true;
    return (STOP_COUNT-1-index-SAFE_SLOT_COUNT > other->lockIndex);
}

void bj::Scheduler::Robot::moveNext()
{
    if (mode == FREE) return;
    bool finished = true;
    if (mode == FORWARD)
    {
        for (int i=0;i<STOP_COUNT; i++)
        {
            auto& action = actions[i];
            if (!action.done())
            {
                finished = false;
                if (!action.pouring && isUnlocked(i)) motor->moveTo(action.distance);
                break;
            }
        }
    }
    else if (mode == BACKWARD)
    {
        for (int i=STOP_COUNT-1;i>=0; i--)
        {
            auto& action = actions[i];
            if (!action.done())
            {
                finished = false;
                if (!action.pouring) motor->moveTo(action.distance);
                break;
            }
        }
    }

    if (finished) motor->moveTo(0);
}

void bj::Scheduler::Robot::valveClosed(uint16_t tank)
{
    if (mode == FREE) return;
    for (auto i = 0; i<STOP_COUNT; i++)
    {
        auto& action = actions[i];
        if (action.pouring)
        {
            if (action.back->port() == tank) action.vBack = 0;
            else if (action.front->port() == tank) action.vFront = 0;
            if (action.done())
            {
                action.pouring = false;
                moveNext();
            }
            break;
        }
    }
}

void bj::Scheduler::Robot::motorStopped(uint32_t position)
{
    if (mode == BACKWARD) //unlock previous stops
    {
        for (auto i = STOP_COUNT-1; i>=0; i--)
        {
            if (actions[i].distance == position || (!actions[i].done()))
            {
                lockIndex = i;
                break;
            }
        }
    }

    if (position == 0)
    {
        if (other->mode == FORWARD) other->switchToBackward();
        reset();
    }
    else
    {
        for (auto i = 0; i< STOP_COUNT;i++)
        {
            if (actions[i].distance == position)
            {
                actions[i].openValves();
                break;
            }
        }
    }
    if (other->mode == FORWARD) other->moveNext();
}

void bj::Scheduler::Robot::reset()
{
    for (int i=0;i<STOP_COUNT; i++)
    {
        actions[i].vBack = 0;
        actions[i].vFront = 0;
        actions[i].pouring = false;
        lockIndex = -1;
        mode = FREE;
    }
}

void bj::Scheduler::Robot::switchToBackward()
{
    mode = BACKWARD;
    for (auto i = STOP_COUNT-1; i>=0;i--) /*lock all related stops*/
    {
        if (!actions[i].done())
        {
            lockIndex = i;
            break;
        }
    }
}
