#include "command.h"
#include "uart/controller.h"
#include "controller.h"
#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

UART_COMMAND(pkp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field pkp = %d",val);
    motor::CONTROLLER.setPKP(val);
}

UART_COMMAND(pki)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field pki = %d",val);
    motor::CONTROLLER.setPKI(val);
}

UART_COMMAND(pkd)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field pkd = %d",val);
    motor::CONTROLLER.setPKD(val);
}

UART_COMMAND(vkp)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field vkp = %d",val);
    motor::CONTROLLER.setVKP(val);
}

UART_COMMAND(vki)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field vki = %d",val);
    motor::CONTROLLER.setVKI(val);
}

UART_COMMAND(kaff)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field kaff = %d",val);
    motor::CONTROLLER.setKAff(val);
}

UART_COMMAND(kvff)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field kvff = %d",val);
    motor::CONTROLLER.setKVff(val);
}

UART_COMMAND(position)
{
	int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("Field position = %d",val);
    //motor::CONTROLLER.setPositionMachine(val << 10);
    motor::CONTROLLER.setPosition(val << 10);
}

UART_COMMAND(speed)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field speed = %d",val);
    motor::CONTROLLER.setSpeed(val);
}

UART_COMMAND(accel)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field accel = %d",val);
    motor::CONTROLLER.setAccel(val);
}

UART_COMMAND(pd)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field pd = %d",val);
    motor::CONTROLLER.setPd(val);
}

UART_COMMAND(id)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field id = %d",val);
    motor::CONTROLLER.setId(val);
}

UART_COMMAND(pq)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field pq = %d",val);
    motor::CONTROLLER.setPq(val);
}

UART_COMMAND(iq)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field iq = %d",val);
    motor::CONTROLLER.setIq(val);
}

UART_COMMAND(calibrate)
{
	uart::CONTROLLER.printMessage("Button 'Calibrate' clicked");
    motor::CONTROLLER.processCommand(motor::CONTROLLER.CALIBRATE);
}

UART_COMMAND(set_zero)
{
	uart::CONTROLLER.printMessage("Button 'Set zero' clicked");
    motor::CONTROLLER.zero = motor::CONTROLLER.ZERO0;
    motor::CONTROLLER.setZero = true;
}

UART_COMMAND(start_motor)
{
	uart::CONTROLLER.printMessage("Button 'Start Motor' clicked");
    motor::CONTROLLER.startMotor();
}

UART_COMMAND(stop_motor)
{
    uart::CONTROLLER.printfMessage("Button 'Stop Motor' clicked");
    motor::CONTROLLER.stopMotor();
}

UART_COMMAND(read_drv)
{
	uart::CONTROLLER.printMessage("Button 'Read DRV' clicked");
}

UART_COMMAND(openvalve)
{
	uart::CONTROLLER.printMessage("Button 'OpenValve' clicked");
}

UART_COMMAND(valve)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field valve = %d",val);
    machine::CONTROLLER.option(val);
}

UART_COMMAND(time)
{
	int32_t val = DECODE_VAL;
	uart::CONTROLLER.printfMessage("Field time = %d",val);
    machine::CONTROLLER.timeVal = val*50;
}

plot::Controller plot::CONTROLLER;
UART_COMMAND_BEGIN
	COMMAND_REG(9, pkp)
	COMMAND_REG(8, pki)
	COMMAND_REG(7, pkd)
	COMMAND_REG(21, vkp)
	COMMAND_REG(24, vki)
	COMMAND_REG(23, kaff)
	COMMAND_REG(22, kvff)
	COMMAND_REG(6, position)
	COMMAND_REG(10, speed)
	COMMAND_REG(11, accel)
	COMMAND_REG(15, pd)
	COMMAND_REG(14, id)
	COMMAND_REG(13, pq)
	COMMAND_REG(12, iq)
	COMMAND_REG(18, calibrate)
	COMMAND_REG(25, set_zero)
	COMMAND_REG(17, start_motor)
	COMMAND_REG(16, stop_motor)
	COMMAND_REG(20, read_drv)
	COMMAND_REG(39, openvalve)
	COMMAND_REG(45, valve)
	COMMAND_REG(30, time)
UART_COMMAND_END
