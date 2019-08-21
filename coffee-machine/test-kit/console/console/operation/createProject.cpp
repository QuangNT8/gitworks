#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, CreateProject)
{
    qDebug()<<"Build CreateProject Request";
    auto ret = new comm::kit::CreateProject::Request();
    ret->set_name(name.toStdString());
    ret->set_total(total);
    if (tankSize != 0) ret->set_tanksize(tankSize);
    return ret;
}

TCP_REPLY(kit, CreateProject)
{
    qDebug()<<"Process CreateProject Reply";
    if (reply.type() == comm::OK)
    {
        comm::kit::CreateProject::Reply r = reply.GetExtension(comm::kit::CreateProject::Reply::EXT);
        auto manager = kit::Manager::instance();
        manager->projects->add(new kit::Project(r.project()));
    }

    return reply.type();
}
