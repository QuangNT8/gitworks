#include "operations.h"

TCP_REQUEST(machine, ChangeTank)
{
    qDebug()<<"Build ChangeTank Request";
    auto ret = new comm::machine::ChangeTank::Request();
    ret->set_index(index);
    ret->set_liquid(liquid);
    ret->set_volume(volume);
    return ret;
}

TCP_REPLY(machine, ChangeTank)
{
    qDebug()<<"Process ChangeTank Reply";
    return reply.type();
}
