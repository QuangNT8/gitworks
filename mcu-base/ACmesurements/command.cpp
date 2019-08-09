#include "command.h"
#include "../uart/controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(button_1)
{
	uart::CONTROLLER.printMessage("Button 'button 1' clicked");
	/*TODO: implement*/
}

UART_COMMAND(button_2)
{
	uart::CONTROLLER.printMessage("Button 'button 2' clicked");
	/*TODO: implement*/
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(5, button_1)
	COMMAND_REG(4, button_2)
UART_COMMAND_END
