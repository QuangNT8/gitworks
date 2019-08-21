#ifndef RCSERVO_H
#define RCSERVO_H


#include "stm32f0xx_hal_gpio.h"

#define init_pos 16
#define open_pos 26

typedef void (* RCservo_event_handler_t)(uint32_t event_type);

typedef enum
{
    Valve1      = GPIO_PIN_0,
    Valve2      = GPIO_PIN_1,
    Valve3      = GPIO_PIN_2,
    Valve4      = GPIO_PIN_3,
    Valve5      = GPIO_PIN_8,
    Valve6      = GPIO_PIN_9,
    Valve7      = GPIO_PIN_10,
    Valve8      = GPIO_PIN_11,
    Valve9      = GPIO_PIN_12,
    Valve10     = GPIO_PIN_13,
    Valve11     = GPIO_PIN_14,
    Valve12     = GPIO_PIN_15,
} ValvePindef;

typedef enum
{
    OFF    = 0x0000U,
    ON     = 0x0001U,
    OFFALL = 0x0002U
} ValveSwitch_st;

typedef enum
{
    left    = 1U,
    right   = 0U
} ValveDir_st;

namespace RCservo
{
    class Controller
    {
    public:
        void init();
        void initTimer6();
        void loop(uint16_t valvest);
        void pulseUp();
        void pulseDown();
        void valveSwitch(ValvePindef pinvalve,ValveSwitch_st valvest,ValveDir_st dir);
        void valve_toggle(uint16_t numbervalve);
        void RCservo_controller();
    private:
      
    };

extern RCservo::Controller CONTROLLER;

}
#endif // CONTROLLER_H
