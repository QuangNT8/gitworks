#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, BuildNetwork)
{
    qDebug()<<"Build BuildNetwork Request";
    auto ret = new comm::kit::BuildNetwork::Request();
    ret->set_id(project->id());
    return ret;
}

TCP_REPLY(kit, BuildNetwork)
{
    qDebug()<<"Process BuildNetwork Reply";
    return reply.type();
}
