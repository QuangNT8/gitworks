#include "operations.h"
TCP_REQUEST(machine, UpdateDebugMode)
{
    qDebug()<<"Build UpdateDebugMode Request";
    auto ret = new comm::machine::UpdateDebugMode::Request();
    ret->set_value(value);
    return ret;
}

TCP_REPLY(machine, UpdateDebugMode)
{
    qDebug()<<"Process UpdateDebugMode Reply";
    return reply.type();
}
