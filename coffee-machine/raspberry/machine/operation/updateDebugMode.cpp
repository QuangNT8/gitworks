#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, UpdateDebugMode)
{
    DLOG(INFO)<< "Execute request UpdateDebugMode";
    auto manager = bj::Manager::instance();
    manager->changeDebugMode(request.value());
    return 0;
}
