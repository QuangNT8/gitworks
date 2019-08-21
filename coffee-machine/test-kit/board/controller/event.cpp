#include "valve/controller.h"
#include "kit/kit-controller.h"
#include "uart/controller.h"
#include "uart2/controller.h"
#include "command.h"

void valve::Controller::valveOpened_()
{
    uart::CONTROLLER.sendMessage(Tkit::ValveOpened, 0, 0);
}

void valve::Controller::valveClosed_()
{
    uart::CONTROLLER.sendMessage(Tkit::ValveClosed, 0, 0);
}

void kit::Controller::pumpClosed_()
{
    uart::CONTROLLER.sendMessage(Tkit::PumpClosed, 0, 0);
}

void kit::Controller::pumpOpened_()
{
    uart::CONTROLLER.sendMessage(Tkit::PumpOpened, 0, 0);
}

void scale_send(uint8_t* data)
{
    uart::CONTROLLER.sendMessage(Tkit::WeightChanged, 17, data);
}

void uart2::Controller::scaleData_()
{
    scale_send(rxBuffer_);
}






