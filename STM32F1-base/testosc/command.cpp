#include "command.h"
#include "uart/controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(accel)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field accel = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(jerk)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field jerk = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(qkp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field qkp = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(qki)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field qki = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(dkp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field dkp = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(dki)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field dki = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(align_motor)
{
	uart::CONTROLLER.printMessage("Button 'Align Motor' clicked");
	/*TODO: implement*/
}

UART_COMMAND(start_motor)
{
	uart::CONTROLLER.printMessage("Button 'Start Motor' clicked");
	/*TODO: implement*/
}

UART_COMMAND(stop_motor)
{
	uart::CONTROLLER.printMessage("Button 'Stop Motor' clicked");
	/*TODO: implement*/
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(312, accel)
	COMMAND_REG(313, jerk)
	COMMAND_REG(316, qkp)
	COMMAND_REG(315, qki)
	COMMAND_REG(311, dkp)
	COMMAND_REG(310, dki)
	COMMAND_REG(307, align_motor)
	COMMAND_REG(0, start_motor)
	COMMAND_REG(314, stop_motor)
UART_COMMAND_END