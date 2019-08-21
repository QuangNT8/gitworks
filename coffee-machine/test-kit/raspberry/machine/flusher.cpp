#include "manager.h"

void kit::Flusher::init(Tank::pointer tank, Pump::pointer pump)
{
    tank_ = tank; pump_ = pump; state_ = STOPPED;
}

void kit::Flusher::start()
{
    DLOG(INFO) << "Start flushing liquid";
    old_weight_ = MAX_WEIGHT;
    flush();
}

void kit::Flusher::weightChanged(double weight, bool stable)
{
//    DLOG(INFO) << "weightChanged";
    weight_ = weight; stable_ = stable;

    if(state_ == FLUSHED)
    {
        DLOG(INFO) << "old_weight_ " << old_weight_ << " weight_" << weight_ << " stable_ " << stable_;
        if(old_weight_ - weight_ >= 5.0)
        {
            old_weight_ = weight_;
            flush();
        }
        else state_ = ZERO_SETTING;
    }

    if(state_ == ZERO_SETTING && stable)
    {
        zero_offset_ = weight_;
        state_ = STOPPED;

        DLOG(INFO) << "weight_zero = "<< zero_offset_;
        Manager::instance()->flushingCompleted(zero_offset_);
    }
}

void kit::Flusher::valveClosed()
{
    DLOG(INFO) << "valveClosed";
    DLOG(INFO) << "change to FLUSHED";
    state_ = FLUSHED;
}

void kit::Flusher::flush()
{
    DLOG(INFO) << "Flush liquid";
    DLOG(INFO) << "change to FLUSHING";
    state_ = FLUSHING;
    tank_->open(FLUSHING_TIME);
}
