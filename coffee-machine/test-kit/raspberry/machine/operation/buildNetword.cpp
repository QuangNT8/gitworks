#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(kit, BuildNetwork)
{
    DLOG(INFO)<< "Execute request BuildNetwork";
    auto manager = Manager::instance();
    if (!manager->ready()) return comm::REQUEST_INVALID;

    manager->changeProject(request.id());
    manager->build(request.id());
    return 0;
}
