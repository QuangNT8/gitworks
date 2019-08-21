#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(kit, OpenValue)
{
    DLOG(INFO)<< "Execute request OpenValue " << request.projectid() << " " << request.value();
    auto manager = kit::Manager::instance();
    if (!manager->ready()) return comm::REQUEST_INVALID;
    if(request.projectid() == "") //open by time
    {
        manager->tank()->open(request.value());
    }
    else
    {
        auto t = manager->predict(request.value(), request.projectid());
        if (t > 0) manager->tank()->open(t);
        else return comm::kit::OpenValue_Error::OpenValue_Error_VALUE_ERROR;
    }
    return 0;
}

SERVICE_EXECUTE(kit, FlushTank)
{
    DLOG(INFO)<< "Execute request FlushTank";
    auto manager = kit::Manager::instance();
    if (!manager->ready()) return comm::REQUEST_INVALID;
    manager->flush(kit::Manager::STOPPED);
    return 0;
}

SERVICE_EXECUTE(kit, ReFillTank)
{
    DLOG(INFO)<< "Execute request ReFillTank";
    auto manager = kit::Manager::instance();
    if (!manager->ready()) return comm::REQUEST_INVALID;
    manager->refill();
    return 0;
}
