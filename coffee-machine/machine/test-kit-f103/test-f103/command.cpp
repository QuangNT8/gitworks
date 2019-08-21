#include "command.h"
#include "uart/controller.h"
#include "controller.h"
#include "valve/controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(field_1)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_1 = %d",val);
    valve::CONTROLLER.open(val*50);
}

UART_COMMAND(togglepump)
{
	uart::CONTROLLER.printMessage("Button 'togglePump' clicked");
    kit::CONTROLLER.togglePump();
}

UART_COMMAND(closepump)
{
	uart::CONTROLLER.printMessage("Button 'closePump' clicked");
    kit::CONTROLLER.closePump();
}

UART_COMMAND(openpump)
{
	uart::CONTROLLER.printMessage("Button 'openPump' clicked");
    kit::CONTROLLER.openPump();
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(3, field_1)
	COMMAND_REG(6, togglepump)
	COMMAND_REG(5, closepump)
	COMMAND_REG(4, openpump)
UART_COMMAND_END
