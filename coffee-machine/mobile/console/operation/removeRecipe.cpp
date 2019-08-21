#include "operations.h"

TCP_REQUEST(machine, RemoveRecipe)
{
    qDebug()<<"Build RemoveRecipe Request";
    auto ret = new comm::machine::RemoveRecipe::Request();
    ret->set_id(recipe->id());
    return ret;
}

TCP_REPLY(machine, RemoveRecipe)
{
    qDebug()<<"Process RemoveRecipe Reply";
    if (reply.type() == comm::OK)
    {
        auto manager = console::Manager::instance();
        auto t = manager->recipes->get(recipe->id());
        if (t != nullptr)
        {
            manager->recipes->remove(recipe->id());
            manager->sendLog("D", "Removed recipe "+ recipe->get_name());
        }
    }
    return reply.type();
}
