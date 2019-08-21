#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(kit, StopProject)
{
    DLOG(INFO)<< "Execute request StopProject";
    Manager::instance()->stop();
    return 0;
}
