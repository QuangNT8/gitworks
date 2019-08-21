#include "operations.h"
#include "models.h"

TCP_REQUEST(machine, Login)
{
    qDebug()<<"Build Login Request";
    auto ret = new comm::machine::Login::Request();
    ret->set_type(2);
    return ret;
}

TCP_REPLY(machine, Login)
{
    qDebug()<<"Process Login Reply";
    if (reply.type() == comm::OK)
    {
        comm::machine::Login::Reply r = reply.GetExtension(comm::machine::Login::Reply::EXT);
        auto manager = home::Manager::instance();
        qDebug() << "Recipes " << r.recipes_size();
        qDebug() << "Liquids " << r.liquids_size();
        for (auto& re: r.recipes()) manager->recipes->add(new home::Recipe(re));
        for (auto& l: r.liquids()) manager->liquids->add(new home::Liquid(l));
        if (r.has_session())
        {
            manager->session->data().CopyFrom(r.session());
        }
        else
        {
            //TODO: for no session
        }
        emit manager->logged();
    }
    return reply.type();
}
