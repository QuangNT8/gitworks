#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(machine, OrderStateChanged)
{
    qDebug()<<"Event execute MessageReceived";
    comm::machine::OrderStateChanged event;
    if (event.ParseFromArray(data, size))
    {
        qDebug()<<"MessageReceived " << event.state();
        emit home::Manager::instance()->orderStateChanged(event.state());
    }
}
