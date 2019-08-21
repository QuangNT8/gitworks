#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, RemoveProject)
{
    qDebug()<<"Build RemoveProject Request";
    auto ret = new comm::kit::RemoveProject::Request();
    ret->set_id(project->id());
    return ret;
}

TCP_REPLY(kit, RemoveProject)
{
    qDebug()<<"Process RemoveProject Reply";
    if (reply.type() == comm::OK)
    {
        comm::kit::RemoveProject::Reply r = reply.GetExtension(comm::kit::RemoveProject::Reply::EXT);
        auto manager = kit::Manager::instance();
        manager->projects->remove(project->id());
        if (manager->currentId == project->id().c_str())
        {
            manager->set_currentId("");
        }
    }

    return reply.type();
}
