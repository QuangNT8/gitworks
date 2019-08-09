#include "command.h"
#include "uart/controller.h"
#include "motor.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(speed)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field speed = %d",val);
    step::MOTOR.setSpeed(val);
}

UART_COMMAND(valveoff)
{
    plot::CONTROLLER.valveisclicked = 0;
	uart::CONTROLLER.printMessage("Button 'ValveOFF' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve1)
{
    plot::CONTROLLER.valveisclicked = 1;
    uart::CONTROLLER.printMessage("Button 'Valve1' clicked");
    /*TODO: implement*/
}

UART_COMMAND(valve2)
{
    plot::CONTROLLER.valveisclicked = 2;
    uart::CONTROLLER.printMessage("Button 'Valve2' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve3)
{
    plot::CONTROLLER.valveisclicked = 3;
    uart::CONTROLLER.printMessage("Button 'Valve3' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve4)
{
    plot::CONTROLLER.valveisclicked = 4;
    uart::CONTROLLER.printMessage("Button 'Valve4' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve5)
{
    plot::CONTROLLER.valveisclicked = 5;
    uart::CONTROLLER.printMessage("Button 'Valve5' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve6)
{
    plot::CONTROLLER.valveisclicked = 6;
    uart::CONTROLLER.printMessage("Button 'Valve6' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve7)
{
    plot::CONTROLLER.valveisclicked = 7;
	uart::CONTROLLER.printMessage("Button 'Valve7' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve8)
{
    plot::CONTROLLER.valveisclicked = 8;
	uart::CONTROLLER.printMessage("Button 'Valve8' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve9)
{
    plot::CONTROLLER.valveisclicked = 9;
	uart::CONTROLLER.printMessage("Button 'Valve9' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve10)
{
    plot::CONTROLLER.valveisclicked = 10;
	uart::CONTROLLER.printMessage("Button 'Valve10' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve11)
{
    plot::CONTROLLER.valveisclicked = 11;
	uart::CONTROLLER.printMessage("Button 'Valve11' clicked");
	/*TODO: implement*/
}

UART_COMMAND(valve12)
{
    plot::CONTROLLER.valveisclicked = 12;
	uart::CONTROLLER.printMessage("Button 'Valve12' clicked");
	/*TODO: implement*/
}

UART_COMMAND(start_motor)
{
	uart::CONTROLLER.printMessage("Button 'Start Motor' clicked");
    step::MOTOR.startMotor();
}

UART_COMMAND(stop_motor)
{
	uart::CONTROLLER.printMessage("Button 'Stop Motor' clicked");
    step::MOTOR.stopMotor();
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(6, speed)
	COMMAND_REG(32, valveoff)
	COMMAND_REG(31, valve1)
	COMMAND_REG(30, valve2)
	COMMAND_REG(29, valve3)
	COMMAND_REG(28, valve4)
	COMMAND_REG(27, valve5)
	COMMAND_REG(26, valve6)
	COMMAND_REG(25, valve7)
	COMMAND_REG(24, valve8)
	COMMAND_REG(23, valve9)
	COMMAND_REG(22, valve10)
	COMMAND_REG(21, valve11)
	COMMAND_REG(20, valve12)
	COMMAND_REG(17, start_motor)
	COMMAND_REG(16, stop_motor)
UART_COMMAND_END
