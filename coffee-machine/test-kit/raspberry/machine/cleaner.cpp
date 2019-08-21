#include "manager.h"

void kit::Cleaner::init(Tank::pointer tank, Pump::pointer pump)
{
    tank_ = tank; pump_ = pump;
}

void kit::Cleaner::start()
{
    DLOG(INFO) << "Start flushing liquid";
    tank_->open(FLUSHING_TIME);
}

void kit::Cleaner::stop()
{
    DLOG(INFO) << "Stop cleaning";
    DLOG(INFO) << "Change to STOPPED";
    pump_->turnOff();
    Manager::instance()->cleaningCompleted();
}

void kit::Cleaner::weightChanged(double weight, bool stable)
{
//    DLOG(INFO) << "weightChanged";

    if(weight >= tank_->size())
    {
        DLOG(INFO) << "Full of liquid " << weight;
        if(pump_->state() == Pump::OPEN) pump_->turnOff();
    }
    if(weight <= 5.0)
    {
        DLOG(INFO) << "Out of liquid " << weight;
        if(pump_->state() == Pump::CLOSED) pump_->turnOn();
    }
}

void kit::Cleaner::valveClosed()
{
    tank_->open(FLUSHING_TIME);
}
