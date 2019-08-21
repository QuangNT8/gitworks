#include "scheduler.h"

#define DLEFT   ((740+50)*1024)
#define DRIGHT  ((740+50)*1024)
#define DVALVE  (420*1024)

void bj::Scheduler::start()
{
    DLOG(INFO) << "Start Scheduler";
    auto manager = bj::Manager::instance();
    left_.motor = manager->leftMotor();
    left_.other = &right_;
    right_.motor = manager->rightMotor();
    right_.other = &left_;

    for (auto i=0;i<STOP_COUNT;i++)
    {
        left_.actions[i].back = manager->getTank(i);
        left_.actions[i].front = manager->getTank(i+STOP_COUNT);
        left_.actions[i].distance = DLEFT + DVALVE*i;
        right_.actions[i].back = manager->getTank(STOP_COUNT-1-i);
        right_.actions[i].front = manager->getTank(2*STOP_COUNT-1-i);
        right_.actions[i].distance = DRIGHT + DVALVE*i;
    }
    this->reset();
}

void bj::Scheduler::reset()
{
    boost::mutex::scoped_lock lock(mutex_);
    left_.reset();
    right_.reset();
}

bj::Scheduler::Result bj::Scheduler::makeDrink(uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients)
{
    boost::mutex::scoped_lock lock(mutex_);
    if (door == bj::LEFT) return left_.createPlan(ingredients);
    else return right_.createPlan(ingredients);
}

bj::Scheduler::Result bj::Scheduler::checkOrder(uint16_t door, const std::vector<std::pair<uint64_t, double>>& ingredients)
{
    if (door == bj::LEFT && left_.mode !=FREE) return BUSY;
    if (door == bj::RIGHT && right_.mode !=FREE) return BUSY;

    uint16_t count = ingredients.size();
    for (auto& it: ingredients)
    {
        for (int i=0;i<STOP_COUNT; i++)
        {
            if (left_.actions[i].back->check(it.first, it.second) || left_.actions[i].front->check(it.first, it.second)) count--;
        }
    }
    if (count == 0) return OK;
    return OUT_OF_LIQUID;
}

void bj::Scheduler::valveClosed(uint16_t tank)
{
    boost::mutex::scoped_lock lock(mutex_);
    left_.valveClosed(tank);
    right_.valveClosed(tank);
}

void bj::Scheduler::motorStopped(uint16_t door, uint32_t position)
{
    boost::mutex::scoped_lock lock(mutex_);
    if (door == bj::LEFT) left_.motorStopped(position);
    else right_.motorStopped(position);
}

void bj::Scheduler::Action::openValves()
{
    pouring = true;
    if (vBack > 0) back->open(200); //FIXME: use neural network here
    if (vFront > 0) front->open(200); //FIXME: use neural network here
}

