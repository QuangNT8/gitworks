#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(kit, LoggedOut)
{
    qDebug()<<"Event execute LoggedOut";
    comm::kit::LoggedOut event;
    if (event.ParseFromArray(data, size))
    {
        emit kit::Manager::instance()->loggedOut();
    }
}
