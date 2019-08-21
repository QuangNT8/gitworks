#include "command.h"

#include "stdio.h"

#include "PWM/PWM.h"
#include "STM32ADC/adc.h"
#include "STM32SPI/spi.h"
#include "MCP_CAN/mcp_can.h"
#include "Stm32_GPIO/gpio.h"

uint16_t i=0;
void plotdata(uint16_t angle)
{
    plot::CONTROLLER.plot_ia(adc::CONTROLLER.adc_ch1);
    plot::CONTROLLER.plot_ib(adc::CONTROLLER.adc_ch2);
    plot::CONTROLLER.plot_angle(angle);
    plot::CONTROLLER.flush();
}

int main()
{
    uint16_t temp;
    GPIO::CONTROLLER.init();
//    adc::CONTROLLER.init();
//    PWM::CONTROLLER.init();

//    spi::CONTROLLER.ledtestinit();
//    spi::CONTROLLER.init();


    uart::CONTROLLER.init();
    plot::CONTROLLER.init();

    MCP_CAN::CONTROLLER.init();
//    PWM::CONTROLLER.setDuties(1100,500,500,500,500);

    while (true)
    {
//        uart::CONTROLLER.printMessage("MCP_can testing..");
        GPIO::CONTROLLER.led_PC9_toggle();
        MCP_CAN::CONTROLLER.loop();
//        MCP_CAN::CONTROLLER.Delay(50);
//        temp = spi::CONTROLLER.loop();
        uart::CONTROLLER.loop();
//        plot::CONTROLLER.loop();
//        plotdata(temp);
    }
//    return 0;
}
