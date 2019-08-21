#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, StopProject)
{
    qDebug()<<"Build StopProject Request";
    auto ret = new comm::kit::StopProject::Request();
    return ret;
}

TCP_REPLY(kit, StopProject)
{
    qDebug()<<"Process StopProject Reply";
    Manager::instance()->set_currentId("");
    return reply.type();
}
