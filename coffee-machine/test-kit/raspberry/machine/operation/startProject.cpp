#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(kit, StartProject)
{
    DLOG(INFO)<< "Execute request StartProject ";
    auto manager = kit::Manager::instance();
    if (!manager->ready()) return comm::REQUEST_INVALID;

    manager->changeProject(request.id());
    manager->run();

    return 0;
}
