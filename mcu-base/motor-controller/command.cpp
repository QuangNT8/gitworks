#include "command.h"
#include "uart/controller.h"
#include "controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(speed)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field speed = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setSpeed(val*10);
}

UART_COMMAND(dkp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field dkp = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setParams_dKP(val);
}

UART_COMMAND(dki)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field dki = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setParams_dKI(val);
}

UART_COMMAND(qkp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field qkp = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setParams_qKP(val);
}

UART_COMMAND(qki)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field qki = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setParams_qKI(val);
}

UART_COMMAND(skp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field skp = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setParams_sKP(val);
}

UART_COMMAND(ski)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field ski = %d",val);
	/*TODO: implement*/
    foc::CONTROLLER.setParams_sKI(val);
}

UART_COMMAND(start_motor)
{
	uart::CONTROLLER.printMessage("Button 'Start motor' clicked");
	/*TODO: implement*/
    foc::CONTROLLER.enableMotor();
}

UART_COMMAND(align_motor)
{
	uart::CONTROLLER.printMessage("Button 'Align motor' clicked");
	/*TODO: implement*/
    foc::CONTROLLER.alignMotor();
}

UART_COMMAND(run_motor)
{
	uart::CONTROLLER.printMessage("Button 'Run motor' clicked");
	/*TODO: implement*/
}

UART_COMMAND(stop_motor)
{
	uart::CONTROLLER.printMessage("Button 'Stop motor' clicked");
	/*TODO: implement*/
    foc::CONTROLLER.disableMotor();
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(16, speed)
	COMMAND_REG(13, dkp)
	COMMAND_REG(12, dki)
	COMMAND_REG(11, qkp)
	COMMAND_REG(10, qki)
	COMMAND_REG(9, skp)
	COMMAND_REG(8, ski)
	COMMAND_REG(18, start_motor)
	COMMAND_REG(19, align_motor)
	COMMAND_REG(28, run_motor)
	COMMAND_REG(17, stop_motor)
UART_COMMAND_END
