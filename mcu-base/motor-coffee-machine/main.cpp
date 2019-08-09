#include "motor.h"
#include "command.h"
#include "uart/controller.h"
#include "RCservoLib/RCservo.h"
#include "stm32f0xx_hal_gpio.h"
int main()
{
    uart::CONTROLLER.init();
    plot::CONTROLLER.init();
    step::MOTOR.init();
    RCservo::CONTROLLER.initTimer6();
    while (true)
    {
        uart::CONTROLLER.loop();
        plot::CONTROLLER.loop();
        step::MOTOR.loop();
        RCservo::CONTROLLER.loop(plot::CONTROLLER.valveisclicked);
    }
    return 0;
}
