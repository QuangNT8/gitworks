#ifndef MCUD_COMMAND_H
#define MCUD_COMMAND_H
#include "uart/controller.h"
namespace plot
{

class Controller
{
public:
    Controller(): _mask(0){}
    void plot_pa1(int16_t v)
    {
        pa1_= v;
        _mask |= 1;
    }
    void plot_pa2(int16_t v)
    {
        pa2_= v;
        _mask |= 2;
    }
    void plot_pa3(int16_t v)
    {
        pa3_= v;
        _mask |= 4;
    }

    void flush()
    {
        if (_mask==0) return;
        uint8_t index = 0;
        _data[index++] = _mask;
        if (_mask & 1)
        {
            _data[index++] = ((pa1_ >> 8) & 0xFF);
            _data[index++] = (pa1_ & 0xFF);
        }
        if (_mask & 2)
        {
            _data[index++] = ((pa2_ >> 8) & 0xFF);
            _data[index++] = (pa2_ & 0xFF);
        }
        if (_mask & 4)
        {
            _data[index++] = ((pa3_ >> 8) & 0xFF);
            _data[index++] = (pa3_ & 0xFF);
        }
        uart::CONTROLLER.sendMessage(2, index,_data);
        _mask = 0;
    }
private:
    uint8_t _mask;
    uint8_t _data[8];
    int16_t pa1_;
    int16_t pa2_;
    int16_t pa3_;
};

extern plot::Controller CONTROLLER;

}

#endif
