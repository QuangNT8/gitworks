#include "command.h"
#include "uart/controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(field_1)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_1 = %d",val);
    plot::CONTROLLER.MotorArray[0]=(uint16_t)val;
	/*TODO: implement*/
}

UART_COMMAND(field_2)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_2 = %d",val);
    plot::CONTROLLER.MotorArray[1]=(uint16_t)val;
	/*TODO: implement*/
}

UART_COMMAND(field_3)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field field_3 = %d",val);
    plot::CONTROLLER.MotorArray[2]=(uint16_t)val;
	/*TODO: implement*/
}

UART_COMMAND(new_button)
{
	uart::CONTROLLER.printMessage("Button 'New Button' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve_1)
{
    plot::CONTROLLER.valveisclicked[0]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 1' clicked %d",plot::CONTROLLER.valveisclicked[0]);
	/*TODO: implement*/
}

UART_COMMAND(valve_2)
{
    plot::CONTROLLER.valveisclicked[1]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 2' clicked %d",plot::CONTROLLER.valveisclicked[1]);
	/*TODO: implement*/
}

UART_COMMAND(valve_3)
{
    plot::CONTROLLER.valveisclicked[2]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 3' clicked %d",plot::CONTROLLER.valveisclicked[2]);
	/*TODO: implement*/
}

UART_COMMAND(valve_4)
{
    plot::CONTROLLER.valveisclicked[3]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 4' clicked %d",plot::CONTROLLER.valveisclicked[3]);
	/*TODO: implement*/
}

UART_COMMAND(valve_5)
{
    plot::CONTROLLER.valveisclicked[8]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 5' clicked %d",plot::CONTROLLER.valveisclicked[8]);
	/*TODO: implement*/
}

UART_COMMAND(valve_6)
{
    plot::CONTROLLER.valveisclicked[9]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 6' clicked %d",plot::CONTROLLER.valveisclicked[9]);
	/*TODO: implement*/
}

UART_COMMAND(valve_7)
{
    plot::CONTROLLER.valveisclicked[10]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 7' clicked %d",plot::CONTROLLER.valveisclicked[10]);
	/*TODO: implement*/
}

UART_COMMAND(valve_8)
{
    plot::CONTROLLER.valveisclicked[11]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 8' clicked %d",plot::CONTROLLER.valveisclicked[11]);
	/*TODO: implement*/
}

UART_COMMAND(valve_9)
{
    plot::CONTROLLER.valveisclicked[12]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 9' clicked %d",plot::CONTROLLER.valveisclicked[12]);
	/*TODO: implement*/
}

UART_COMMAND(valve_10)
{
    plot::CONTROLLER.valveisclicked[13]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 10' clicked %d",plot::CONTROLLER.valveisclicked[13]);
	/*TODO: implement*/
}

UART_COMMAND(valve_11)
{
    plot::CONTROLLER.valveisclicked[14]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 11' clicked %d",plot::CONTROLLER.valveisclicked[14]);
	/*TODO: implement*/
}

UART_COMMAND(valve_12)
{
    plot::CONTROLLER.valveisclicked[15]=1;
    uart::CONTROLLER.printfMessage("Button 'Valve 12' clicked %d",plot::CONTROLLER.valveisclicked[15]);
	/*TODO: implement*/
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(3, field_1)
	COMMAND_REG(2, field_2)
	COMMAND_REG(1, field_3)
	COMMAND_REG(17, new_button)
	COMMAND_REG(27, valve_1)
	COMMAND_REG(26, valve_2)
	COMMAND_REG(25, valve_3)
	COMMAND_REG(24, valve_4)
	COMMAND_REG(23, valve_5)
	COMMAND_REG(22, valve_6)
	COMMAND_REG(21, valve_7)
	COMMAND_REG(20, valve_8)
	COMMAND_REG(19, valve_9)
	COMMAND_REG(18, valve_10)
	COMMAND_REG(5, valve_11)
	COMMAND_REG(4, valve_12)
UART_COMMAND_END
