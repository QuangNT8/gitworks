#ifndef CAN_H
#define CAN_H
#include "stm32f042x6.h"

namespace can
{
    class Controller
    {
    public:
        void init();
        void loop();
        uint8_t Transmit(uint32_t addr, uint32_t data_size, uint8_t * tab_data);
        void delay(uint16_t);
        void SystemClock_Config(void);
    private:
      
    };

extern can::Controller CONTROLLER;

}
#endif // CONTROLLER_H
