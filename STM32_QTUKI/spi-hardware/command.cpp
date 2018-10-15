#include "command.h"
#include "uart/controller.h"
#include "stm32f0xx.h"
#define BSRR_VAL        0x0300
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(field_1)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_1 = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(field_2)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_2 = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(field_3)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_3 = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(on)
{
	uart::CONTROLLER.printMessage("Button 'ON' clicked");
	/*TODO: implement*/
    GPIOA->BRR = 0x8000;
}

UART_COMMAND(off)
{
	uart::CONTROLLER.printMessage("Button 'OFF' clicked");
	/*TODO: implement*/
    GPIOA->BSRR = 0x8000;
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
    COMMAND_REG(3, field_1)
    COMMAND_REG(2, field_2)
    COMMAND_REG(1, field_3)
    COMMAND_REG(4, on)
    COMMAND_REG(5, off)
UART_COMMAND_END
