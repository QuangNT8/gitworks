#ifndef PWM_H
#define PWM_H

#include "stm32f0xx.h"

namespace PWM
{
    class Controller
    {
    public:
        void init();
        void init2();
        void loop();
        void setDuties(uint16_t SW1, uint16_t SW2, uint16_t SW3, uint16_t SW4,uint16_t SW5);
    private:

    };

extern PWM::Controller CONTROLLER;

}
#endif // CONTROLLER_H
