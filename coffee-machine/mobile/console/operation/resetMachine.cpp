#include "operations.h"

TCP_REQUEST(machine, ResetMachine)
{
    qDebug()<<"Build ResetMachine";
    auto ret = new comm::machine::ResetMachine::Request();
    return ret;
}

TCP_REPLY(machine, ResetMachine)
{
    qDebug()<<"Process ResetMachine Reply";
    console::Manager::instance()->sendLog("D","Reset machine");
    return reply.type();
}
