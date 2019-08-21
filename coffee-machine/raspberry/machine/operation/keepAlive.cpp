#include "operations.h"

SERVICE_EXECUTE(machine, KeepAlive)
{
    DLOG(INFO)<< "Execute request KeepAlive ";
    return 0;
}
