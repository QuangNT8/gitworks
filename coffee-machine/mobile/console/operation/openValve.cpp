#include "operations.h"

TCP_REQUEST(machine, OpenValve)
{
    qDebug()<<"Build OpenValve Request";
    auto ret = new comm::machine::OpenValve::Request();
    ret->set_index(index);
    if (time > 0) ret->set_time(time/TIMER_CYCLE);
    else ret->set_vol(vol);
    return ret;
}

TCP_REPLY(machine, OpenValve)
{
    qDebug()<<"Process OpenValve Reply";
    return reply.type();
}
