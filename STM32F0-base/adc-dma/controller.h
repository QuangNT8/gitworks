#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <stdint.h>

namespace adc
{

class Controller
{
public:
    void init();
    void loop();
    Controller(){}
private:
    uint16_t countDelayPlot_;
    uint16_t data_[4];
};

extern adc::Controller CONTROLLER;

}

#endif // CONTROLLER_H
