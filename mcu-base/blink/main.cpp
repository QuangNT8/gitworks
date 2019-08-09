#include "stm32_timer/timer.h"
#include "Stm32_System_clock/osc.h"
#include "Stm32_GPIO/gpio.h"
#include "controller.h"
#include "stdio.h"
#include "Stm32_USART/Usart.h"

#include "stm32f0xx_hal_gpio.h"

void irq_timer(uint32_t event_type)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
}

int main()
{
//        blink::CONTROLLER.init();
//    osc::CONTROLLER.HAL_Init();
    timer::CONTROLLER.init_IT(irq_timer);
    GPIO::CONTROLLER.init();
//    USART::CONTROLLER.init();
    while (true)
    {
//        timer::CONTROLLER.loop();
//        blink::CONTROLLER.loop();
        //printf("quang \r\n");
//        GPIO::CONTROLLER.loop();
        //USART::CONTROLLER.loop();
    }
//    return 0;
}
