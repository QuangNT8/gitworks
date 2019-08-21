#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, OpenValue)
{
    qDebug()<<"Build OpenValue Request";
    auto ret = new comm::kit::OpenValue::Request();
    ret->set_value(value);
    ret->set_projectid(project != nullptr ? project->id(): "");
    return ret;
}

TCP_REPLY(kit, OpenValue)
{
    qDebug()<<"Process OpenValue Reply";
    return reply.type();
}

TCP_REQUEST(kit, FlushTank)
{
    qDebug()<<"Build FlushTank Request";
    auto ret = new comm::kit::FlushTank::Request();
    return ret;
}

TCP_REPLY(kit, FlushTank)
{
    qDebug()<<"Process FlushTank Reply";
    return reply.type();
}
TCP_REQUEST(kit, ReFillTank)
{
    qDebug()<<"Build ReFillTank Request";
    auto ret = new comm::kit::ReFillTank::Request();
    return ret;
}

TCP_REPLY(kit, ReFillTank)
{
    qDebug()<<"Process ReFillTank Reply";
    return reply.type();
}
