#ifndef GPIO_H
#define GPIO_H

#include "stm32f103xb.h"
//#include "stm32f0xx_hal_dma.h"
//#include "stm32f0xx_hal_tim.h"


namespace GPIO
{
    class Controller
    {
    public:
        void init();
        void loop();
        void led_PB1_toggle();
    private:
      
    };

extern GPIO::Controller CONTROLLER;

}
#endif // CONTROLLER_H
