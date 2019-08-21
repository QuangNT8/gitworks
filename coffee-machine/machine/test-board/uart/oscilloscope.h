#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H
#include <stdint.h>
#include "controller.h"

namespace plot
{

class Oscilloscope
{
public:
    enum State{ ADDING, PROBING, FLUSHING, FLUSHED};

    void setup(uint8_t plot)
    {
        index_ = 0;
        total_ = 0;
        state_ = PROBING;
        threshold_ = 0;
        lastVal_ = -32000;
        this->plot_ = plot;
        int16_t min_ = 32000;
        int16_t max_ = -32000;
    }

    void setThreshold(int16_t t){threshold_ = t;}

    void addValue2(int16_t v, uint16_t s)
    {
        if (state_ == PROBING)
        {
            if (s == ADDING)
            {
                state_ = ADDING;
                countIndex_ = 0;
                data_[0] = 1;
                data_[1] = ((v>>8) & 0xFF);
                data_[2] = (v & 0xFF);
                index_ = 3;
                total_ = 1;
            }
        }
        else if (state_ == ADDING)
        {
            int16_t d = v - lastVal_;

            if ((d > 127) || (d < -127) || (data_[countIndex_] > 80))
            {
                countIndex_ = index_;
                data_[index_++] = 1;
                data_[index_++] = ((v >> 8)& 0xFF);
                data_[index_++] = (v & 0xFF);
            }
            else
            {
                data_[countIndex_]++;
                data_[index_++] = d;
            }
            total_++;
            if ((total_ > 1000) || (index_ > 1199) || (s==FLUSHING))
            {
                state_ = FLUSHING;
                index_ = 0;
                total_ = 0;
            }
        }
        lastVal_ = v;
    }

    uint16_t addValue(int16_t v)
    {
        switch (state_)
        {
            case PROBING:
                if ((lastVal_< threshold_) && (v >= threshold_))
                {
                    state_ = ADDING;
                    countIndex_ = 0;
                    data_[0] = 1;
                    data_[1] = ((v>>8) & 0xFF);
                    data_[2] = (v & 0xFF);
                    index_ = 3;
                    total_ = 1;
                }
                else
                {
                    if (v < min_) min_ = v;
                    if (v > max_) max_ = v;
                    if (index_++ > 1000)
                    {
                        threshold_ = (min_+max_)/2;
                        min_ = 32000;
                        max_ = -32000;
                        index_ = 0;
                        uart::CONTROLLER.printfMessage("Auto detect threshold:%d", threshold_);
                    }
                }
                break;
            case ADDING:
            {
                int16_t d = v - lastVal_;

                if ((d > 127) || (d < -127) || (data_[countIndex_] > 80))
                {
                    countIndex_ = index_;
                    data_[index_++] = 1;
                    data_[index_++] = ((v >> 8)& 0xFF);
                    data_[index_++] = (v & 0xFF);
                }
                else
                {
                    data_[countIndex_]++;
                    data_[index_++] = d;
                }

                total_++;
                if ((total_ > 1000) || (index_ > 1199))
                {
                    state_ = FLUSHING;
                    index_ = 0;
                    total_ = 0;
                }
                break;
            }
        }
        lastVal_ = v;
        return state_;
    }

    inline bool loop(bool done)
    {
        if (state_ == FLUSHING)
        {
            uint8_t count = data_[index_];
            if (uart::CONTROLLER.sendPlot(3,count + 1, total_, plot_, data_ + index_ + 1))
            {
                total_ += count;
                index_ += count+2;
                if ((total_ > 999) || (index_ > 1198))
                {
                    index_ = 0;
                    total_ = 0;
                    state_ = FLUSHED;
                }
            }
            return false;
        }
        else if (state_ == FLUSHED && done)
        {
            state_ = PROBING;
        }
        return true;
    }


private:
    int16_t min_;
    int16_t max_;

    uint8_t data_[1200];
    uint16_t index_;
    uint16_t countIndex_;
    uint16_t total_;
    uint16_t state_;
    int16_t threshold_;
    int16_t lastVal_;
    uint8_t plot_;
};

}

#endif // OSCILLOSCOPE_H
