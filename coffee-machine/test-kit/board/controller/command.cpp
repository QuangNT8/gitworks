#include "uart/controller.h"
#include "command.h"
#include "valve/controller.h"
#include "kit/kit-controller.h"

#define UINT32_DECODE_VAL ((uint32_t)data[0] <<24)| ((uint32_t)data[1] << 16)| ((uint32_t)data[2] <<8) | (uint32_t)data[3]
#define UINT16_DECODE_VAL ((uint16_t)data[0] <<8) | (uint16_t)data[1]

UART_COMMAND(OpenValve)
{
    uint16_t time = UINT16_DECODE_VAL;
    valve::CONTROLLER.open(time);
}

UART_COMMAND(TurnOnPump)
{
   kit::CONTROLLER.openPump();
}

UART_COMMAND(TurnOffPump)
{
   kit::CONTROLLER.closePump();
}


UART_COMMAND_BEGIN
    COMMAND_REG(Tkit::OpenValve, OpenValve)
    COMMAND_REG(Tkit::TurnOnPump, TurnOnPump)
    COMMAND_REG(Tkit::TurnOffPump, TurnOffPump)
UART_COMMAND_END
