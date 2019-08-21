#include "valve/controller.h"
#include "motor/controller.h"
#include "uart/controller.h"
#include "command.h"

void valve::Controller::valveOpened_(uint16_t index)
{
    uint8_t data[2];
    data[0] = ((index>>8) & 0xFF);
    data[1] = (index & 0xFF);
    uart::CONTROLLER.sendMessage(bj::ValveOpened, 2, data);
}

void valve::Controller::valveClosed_(uint16_t index)
{
    uint8_t data[2];
    data[0] = ((index>>8) & 0xFF);
    data[1] = (index & 0xFF);
    uart::CONTROLLER.sendMessage(bj::ValveClosed, 2, data);
}

void motor::Controller::motorStarted_(uint32_t position)
{
    uint8_t data[6];
    data[0] = ((ROBOT>>8) & 0xFF);
    data[1] = (ROBOT & 0xFF);

    data[2] = ((position>>24) & 0xFF);
    data[3] = ((position>>16) & 0xFF);
    data[4] = ((position>>8) & 0xFF);
    data[5] = (position & 0xFF);
    uart::CONTROLLER.sendMessage(bj::MotorStarted, 6, data);
}

void motor::Controller::motorStopped_(uint32_t position)
{
    uint8_t data[6];
    data[0] = ((ROBOT>>8) & 0xFF);
    data[1] = (ROBOT & 0xFF);

    data[2] = ((position>>24) & 0xFF);
    data[3] = ((position>>16) & 0xFF);
    data[4] = ((position>>8) & 0xFF);
    data[5] = (position & 0xFF);
    uart::CONTROLLER.sendMessage(bj::MotorStopped, 6, data);
}



void motor::Controller::positionChanged_(uint32_t position)
{
    uint8_t data[6];
    data[0] = ((ROBOT>>8) & 0xFF);
    data[1] = (ROBOT & 0xFF);

    data[2] = ((position>>24) & 0xFF);
    data[3] = ((position>>16) & 0xFF);
    data[4] = ((position>>8) & 0xFF);
    data[5] = (position & 0xFF);
    uart::CONTROLLER.sendMessage(bj::PositionChanged, 6, data);
}

void motor::Controller::motorReady_()
{
    uint8_t data[2];
    data[0] = ((ROBOT>>8) & 0xFF);
    data[1] = (ROBOT & 0xFF);
    uart::CONTROLLER.sendMessage(bj::MachineReady, 2, data);
}

void motor::Controller::motorError_()
{
    uint8_t data[2];
    data[0] = ((ROBOT>>8) & 0xFF);
    data[1] = (ROBOT & 0xFF);
    uart::CONTROLLER.sendMessage(bj::MachineError, 2, data);
}



