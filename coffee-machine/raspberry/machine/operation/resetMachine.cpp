#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, ResetMachine)
{
    DLOG(INFO)<< "Execute request ResetMachine";
    bj::Manager::instance()->resetMachine();
    return 0;
}
