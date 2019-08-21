#include "operations.h"
#include "models.h"
#include "core/views.h"

TCP_REQUEST(machine, Login)
{
    qDebug()<<"Build Login Request";
    auto ret = new comm::machine::Login::Request();
    ret->set_type(1);
    return ret;
}

TCP_REPLY(machine, Login)
{
    qDebug()<<"Process Login Reply";
    if (reply.type() == comm::OK)
    {
        comm::machine::Login::Reply r = reply.GetExtension(comm::machine::Login::Reply::EXT);
        auto manager = console::Manager::instance();
        for (auto& re: r.recipes()) manager->recipes->add(new console::Recipe(re));
        for (auto& l: r.liquids()) manager->liquids->add(new console::Liquid(l));
        for (auto& t: r.tanks())
        {
            for (auto it : manager->tanks->data())
            {
                auto tank = qobject_cast<console::Tank*>(it);
                if (tank->get_index() == t.index())
                {
                    tank->set_volume(t.volume());
                    tank->set_liquidid(t.liquidid());
                    if (t.has_pouring()) tank->set_pouring(t.pouring());
                    break;
                }
            }
        }
        manager->leftMotor->set_position(r.leftmotors().position());
        manager->rightMotor->set_position(r.rightmotors().position());
        manager->set_debugMode(r.debugmode());
        manager->set_machineReady(r.machineready());
        emit manager->logged();
    }

    return reply.type();
}
