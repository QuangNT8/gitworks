#include "operations.h"

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
    return ret;
}

TCP_REPLY(machine, CheckOrder)
{
    qDebug()<<"Process CheckOrder Reply";

    return reply.type();
}
