#include"stm32f042x6.h"

#include "uart/controller.h"
#include "Stm32_GPIO/gpio.h"
#include "controller.h"
#include "stdio.h"
#include "CAN/can.h"


uint8_t datatest[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};


int main()
{
    uint32_t count_main=0;
//        blink::CONTROLLER.init();
//    osc::CONTROLLER.HAL_Init();
//    timer::CONTROLLER.init_IT(irq_timer);
    GPIO::CONTROLLER.init();
    uart::CONTROLLER.init();
    can::CONTROLLER.init();
    while (true)
    {
        if(count_main>20000)
        {
            uart::CONTROLLER.printfMessage("sent to 0x%x",0x01);
            can::CONTROLLER.Transmit(0x01,8,datatest);
            count_main=0;
        }
        else {
            count_main++;
        }
//        timer::CONTROLLER.loop();
//        blink::CONTROLLER.loop();
//        uart::CONTROLLER.printfMessage("RxHeader.StdId 0x%x",0);

        GPIO::CONTROLLER.loop();
//        can::CONTROLLER.delay(500);
        can::CONTROLLER.loop();
//        GPIO::CONTROLLER.loop();
        uart::CONTROLLER.loop();
        //USART::CONTROLLER.loop();
    }
//    return 0;
}
