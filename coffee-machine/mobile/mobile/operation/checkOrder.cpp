#include "operations.h"
#include "models.h"

TCP_REQUEST(machine, CheckOrder)
{
    qDebug()<<"Build CheckOrder Request";
    auto ret = new comm::machine::CheckOrder::Request();
    ret->set_door(door);

    uint32_t totalVolume = 0;
    for(auto &it: ingredients) totalVolume += static_cast<uint32_t>(it.toMap().value("volume").toInt());
    for(auto &it : ingredients)
    {
        auto add = ret->add_ingredients();
        add->set_liquidid(static_cast<uint64_t>(it.toMap().value("liquidid").toInt()));
        add->set_volume((volume * static_cast<uint32_t>(it.toMap().value("volume").toInt())) / totalVolume);
    }

    qDebug() << "Door:" << ret->door();
    for (auto& it: ret->ingredients())
    {
        qDebug() << "ingredients:" << it.liquidid() << it.volume();
    }
    return ret;
}

TCP_REPLY(machine, CheckOrder)
{
    qDebug()<<"Process CheckOrder Reply" << reply.type();
    if (reply.type() == comm::OK)
    {
        comm::machine::CheckOrder::Reply r = reply.GetExtension(comm::machine::CheckOrder::Reply::EXT);
    }

    return reply.type();
}
