#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(kit, MessageReceived)
{
    qDebug()<<"Event execute MessageReceived";
    comm::kit::MessageReceived event;
    if (event.ParseFromArray(data, size))
    {
        Manager::instance()->sendLog(event.text().c_str());
    }
}
