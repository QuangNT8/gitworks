#include "command.h"
#include "uart/controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

UART_COMMAND(new_field)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field new_field = %d",val);
	/*TODO: implement*/
}

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

UART_COMMAND(new_button)
{
	uart::CONTROLLER.printMessage("Button 'New Button' clicked");
	/*TODO: implement*/
}

UART_COMMAND(new_button)
{
	uart::CONTROLLER.printMessage("Button 'New Button' clicked");
	/*TODO: implement*/
}

UART_COMMAND(new_button)
{
	uart::CONTROLLER.printMessage("Button 'New Button' clicked");
	/*TODO: implement*/
}

UART_COMMAND(button_1)
{
	uart::CONTROLLER.printMessage("Button 'Button 1' clicked");
	/*TODO: implement*/
}

UART_COMMAND(button_2)
{
	uart::CONTROLLER.printMessage("Button 'Button 2' clicked");
	/*TODO: implement*/
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(14, new_field)
	COMMAND_REG(13, new_field)
	COMMAND_REG(12, new_field)
	COMMAND_REG(11, new_field)
	COMMAND_REG(10, new_field)
	COMMAND_REG(9, new_field)
	COMMAND_REG(8, new_field)
	COMMAND_REG(7, new_field)
	COMMAND_REG(6, new_field)
	COMMAND_REG(3, field_1)
	COMMAND_REG(2, field_2)
	COMMAND_REG(1, field_3)
	COMMAND_REG(17, new_button)
	COMMAND_REG(16, new_button)
	COMMAND_REG(15, new_button)
	COMMAND_REG(5, button_1)
	COMMAND_REG(4, button_2)
UART_COMMAND_END
