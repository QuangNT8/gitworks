#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, UpdateLiquid)
{
    DLOG(INFO)<< "Execute request UpdateLiquid";
    auto manager = bj::Manager::instance();
    if (request.liquid().id() == 0)
    {
        auto liquid = manager->data().add_liquids();
        liquid->set_id(manager->nextId());
        liquid->set_name(request.liquid().name());
        liquid->mutable_network()->CopyFrom(request.liquid().network());

        reply.mutable_liquid()->CopyFrom(request.liquid());
        reply.mutable_liquid()->set_id(liquid->id());
        DLOG(INFO)<< "Add liquid " << liquid->id() << " " << liquid->name();
        return 0;
    }
    else
    {
        for (auto i = 0; manager->data().liquids_size(); i++)
        {
            auto liquid = manager->data().mutable_liquids(i);
            if (liquid->id() == request.liquid().id())
            {
                liquid->CopyFrom(request.liquid());
                manager->saveData();
                reply.mutable_liquid()->CopyFrom(request.liquid());
                manager->updateNetwork(liquid->id());
                DLOG(INFO)<< "Edit liquid " << liquid->id() << " " << liquid->name();
                return 0;
            }
        }
        return comm::REQUEST_INVALID;
    }
}
