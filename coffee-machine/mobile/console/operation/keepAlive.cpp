#include "operations.h"

TCP_REQUEST(machine, KeepAlive)
{
    qDebug()<<"Build KeepAlive Request";
    auto ret = new comm::machine::KeepAlive::Request();
    return ret;
}

TCP_REPLY(machine, KeepAlive)
{
    qDebug()<<"Process KeepAlive Reply";
    return reply.type();
}
