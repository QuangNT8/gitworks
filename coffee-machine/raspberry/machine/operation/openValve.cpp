#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, OpenValve)
{
    DLOG(INFO)<< "Execute request OpenValve ";

    auto tank = bj::Manager::instance()->getTank(request.index());
    if (tank == nullptr) return comm::machine::OpenValve::TANK_INVALID;

    if (request.has_time())
    {
        DLOG(INFO)<< "OpenValve by time " << request.time();
        tank->open(request.time());
    }
    else
    {
        DLOG(INFO)<< "OpenValve by vol " << request.vol();
        ann::Network* net = bj::Manager::instance()->network(tank->liquid());
        if(net != nullptr)
        {
            vector<double> input;
            input.push_back(tank->volume());
            input.push_back(request.vol());
            uint16_t time = static_cast<uint16_t>(net->predict(input)[0]);
            tank->open(time);
        }
    }

    return 0;
}
