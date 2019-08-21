#include "operations.h"
#include "models.h"

TCP_REQUEST(kit, Login)
{
    qDebug()<<"Build Login Request";
    auto ret = new comm::kit::Login::Request();
    return ret;
}

TCP_REPLY(kit, Login)
{
    qDebug()<<"Process Login Reply";
    if (reply.type() == comm::OK)
    {
        comm::kit::Login::Reply r = reply.GetExtension(comm::kit::Login::Reply::EXT);
        qDebug() << "Project size:" << r.projects_size() << r.state();
        auto manager = kit::Manager::instance();

        for (auto& p: r.projects()) manager->projects->add(new kit::Project(p));
        if (r.has_currentproject()) manager->set_currentId(r.currentproject().c_str());
        else manager->set_currentId("");
        manager->set_state(r.state());

        for (auto&p: manager->projects->data())
        {
            auto pro = qobject_cast<kit::Project*>(p);
            qDebug() << pro->get_name() << pro->data().samples_size();
        }

        emit manager->logged();
    }

    return reply.type();
}
