#include "manager.h"

void kit::Refiller::init(Tank::pointer tank, Pump::pointer pump)
{
    tank_ = tank; pump_ = pump;
}

void kit::Refiller::start()
{
    DLOG(INFO) << "Start refilling liquid";
    pump_->turnOn();
}

void kit::Refiller::weightChanged(double weight, bool stable)
{
//    DLOG(INFO) << "weightChanged " << weight;

    if(weight >= tank_->size())
    {
        DLOG(INFO) << "Full of liquid " << weight;
        if(pump_->state() == Pump::OPEN)
        {
            pump_->turnOff();
            Manager::instance()->refillingCompleted();
        }
    }
}
