#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(kit, CleanTank)
{
    DLOG(INFO)<< "Execute request CleanTank";
    auto manager = kit::Manager::instance();
    if (!manager->ready()) return comm::REQUEST_INVALID;
    manager->clean();
    return 0;
}
