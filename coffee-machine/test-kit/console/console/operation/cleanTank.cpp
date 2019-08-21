#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, CleanTank)
{
    qDebug()<<"Build CleanTank Request";
    auto ret = new comm::kit::CleanTank::Request();
    return ret;
}

TCP_REPLY(kit, CleanTank)
{
    qDebug()<<"Process CleanTank Reply";
    return reply.type();
}
