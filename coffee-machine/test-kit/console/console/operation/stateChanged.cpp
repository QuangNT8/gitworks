#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(kit, StateChanged)
{
    qDebug()<<"Event execute StateChanged";
    comm::kit::StateChanged event;
    if (event.ParseFromArray(data, size))
    {
        auto manager = Manager::instance();
        manager->set_state(event.state());
        qDebug() << "State" << event.state();
        if (event.has_current())
        {
            qDebug() << "Current project" << event.current().c_str();
            manager->set_currentId(event.current().c_str());
            //TODO: implement for change current
        }
    }
}
