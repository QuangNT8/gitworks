#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(machine, MessageReceived)
{
    qDebug()<<"Event execute MessageReceived";
    comm::machine::MessageReceived event;
    if (event.ParseFromArray(data, size))
        console::Manager::instance()->sendLog("D",event.text().c_str());
}
