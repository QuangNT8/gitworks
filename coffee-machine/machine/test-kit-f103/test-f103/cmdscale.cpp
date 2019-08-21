#include "uart2/controller.h"
#include "uart/controller.h"

void scale_send(uint8_t* data)
{
    uart::CONTROLLER.sendMessage(1, 17, data);
}

void uart2::Controller::processCommand_()
{
    scale_send(rxBuffer_);
}
