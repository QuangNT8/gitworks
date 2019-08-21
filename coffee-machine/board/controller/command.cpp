#include "uart/controller.h"
#include "command.h"
#include "valve/controller.h"
#include "motor/controller.h"

#define UINT32_DECODE_VAL ((uint32_t)data[0] <<24)| ((uint32_t)data[1] << 16)| ((uint32_t)data[2] <<8) | (uint32_t)data[3]
#define UINT16_DECODE_VAL ((uint16_t)data[0] <<8) | (uint16_t)data[1]

UART_COMMAND(OpenValve)
{
    uint16_t time = UINT16_DECODE_VAL;
    uint16_t index = ((uint16_t)data[2] <<8) | (uint16_t)data[3];
    valve::CONTROLLER.open(index, time);
}

UART_COMMAND(MoveMotor)
{
    uint32_t position = UINT32_DECODE_VAL; //FIXME: use UINTxxx
    motor::CONTROLLER.setPosition(position);
}

UART_COMMAND(ResetMachine)
{
    motor::CONTROLLER.reset();
}

UART_COMMAND_BEGIN
    COMMAND_REG(bj::OpenValve, OpenValve)
    COMMAND_REG(bj::MoveMotor, MoveMotor)
    COMMAND_REG(bj::ResetMachine, ResetMachine)
UART_COMMAND_END
