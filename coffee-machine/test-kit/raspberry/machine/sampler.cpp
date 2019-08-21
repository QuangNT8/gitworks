#include "manager.h"

void kit::Sampler::init(Tank::pointer tank, Pump::pointer pump)
{
    tank_ = tank; pump_ = pump; state_ = STOPPED;
    srand(static_cast<unsigned>(time(nullptr)));
}

void kit::Sampler::start()
{
    DLOG(INFO) << "Start sampling";

    if(state_ != STOPPED) DLOG(INFO) << "Sampler is still running!";
    else
    {
        DLOG(INFO) << "Change to FILLING";
        state_ = FILLING;
        pump_->turnOn();
    }
}

void kit::Sampler::stop()
{
    DLOG(INFO) << "Stop sampling";
    if(state_ == STOPPED) DLOG(INFO) << "Sampler is still stopped!";
    else
    {
        DLOG(INFO) << "Change to STOPPED";
        state_ = STOPPED;
        if(pump_->state() == Pump::OPEN) pump_->turnOff();
    }
}

void kit::Sampler::weightChanged(double weight, bool stable)
{
//    DLOG(INFO) << "weightChanged";
    weight_ = weight;
    stable_ = stable;

    if(state_ == FILLING && weight_ >= tank_->size())
    {
        DLOG(INFO) << "Full of liquid " << weight;
        if(pump_->state() == Pump::OPEN) pump_->turnOff();
        else if(pump_->state() == Pump::CLOSED)
        {
            if(stable)
            {
                DLOG(INFO) << "Change to SAMPLING";
                state_ = SAMPLING;
                sample();
            }
        }
    }
    else
    {
        if(state_ == SAMPLED && stable)
        {
            v_out_ = v_tank_ - weight_;
            Manager::instance()->sampleCreated(v_tank_, v_out_expected_, t_valve_, v_out_);

            if(state_ != STOPPED)
            {
                if(weight_ <= OUT_OF_LIQUID)
                {
                    DLOG(INFO) << "Change to FILLING";
                    state_ = FILLING;
                    pump_->turnOn();
                }
                else
                {
                    DLOG(INFO) << "Change to SAMPLING";
                    state_ = SAMPLING;
                    sample();
                }
            }
        }
    }
}

void kit::Sampler::valveClosed()
{
    DLOG(INFO) << "valveClosed";

    if(state_ == SAMPLING)
    {
        DLOG(INFO) << "Change to SAMPLED";
        state_ = SAMPLED;
    }
}

void kit::Sampler::sample()
{
//    DLOG(INFO) << "sample";

    v_out_expected_ = rand() % (MAX_VOLUME_OUT - MIN_VOLUME_OUT + 1) + MIN_VOLUME_OUT;
    t_valve_ = static_cast<uint16_t>(Manager::instance()->predict(v_out_expected_));
    v_tank_ = weight_;
    tank_->open(t_valve_);
}
