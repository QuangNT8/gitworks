#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, StartProject)
{
    qDebug()<<"Build StartProject Request";
    auto ret = new comm::kit::StartProject::Request();
    ret->set_id(project->id());
    return ret;
}

TCP_REPLY(kit, StartProject)
{
    qDebug()<<"Process StartProject Reply";
    return reply.type();
}
