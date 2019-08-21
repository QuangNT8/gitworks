#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, ChangeTank)
{
    DLOG(INFO)<< "Execute request ChangeTank ";

    if(request.index() >= TANK_COUNT) return comm::REQUEST_INVALID;

    auto tank = bj::Manager::instance()->getTank(request.index());
    if (tank != nullptr)
    {
        tank->change(request.liquid(),request.volume());
        comm::machine::StateChanged event;
        auto it = event.add_tanks();
        it->set_index(request.index());
        it->set_volume(request.volume());
        it->set_liquidid(request.liquid());
        it->set_pouring(tank->closed());

        connection_->sendMessage(0, comm::machine::StateChanged::kEXTFieldNumber, event);
    }

    return 0;
}
