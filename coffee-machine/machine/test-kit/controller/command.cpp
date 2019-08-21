#include "command.h"
#include "uart/controller.h"
#include "controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(tvalve)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field tvalve = %d",val);
    testkit::CONTROLLER.openValve(15, val);
}

UART_COMMAND(openpump)
{
	uart::CONTROLLER.printMessage("Button 'openpump' clicked");
    testkit::CONTROLLER.openPump();
}

UART_COMMAND(closepump)
{
	uart::CONTROLLER.printMessage("Button 'closepump' clicked");
    testkit::CONTROLLER.closePump();
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(3, tvalve)
	COMMAND_REG(5, openpump)
	COMMAND_REG(4, closepump)
UART_COMMAND_END
