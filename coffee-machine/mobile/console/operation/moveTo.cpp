#include "operations.h"

TCP_REQUEST(machine, MoveTo)
{
    qDebug()<<"Build MoveTo Request";
    auto ret = new comm::machine::MoveTo::Request();
    ret->set_index(index);
    ret->set_position(position);
    return ret;
}

TCP_REPLY(machine, MoveTo)
{
    qDebug()<<"Process MoveTo Reply";
    return reply.type();
}
