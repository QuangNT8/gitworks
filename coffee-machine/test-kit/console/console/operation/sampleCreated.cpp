#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(kit, SampleCreated)
{
    qDebug()<<"Event execute SampleCreated";
    comm::kit::SampleCreated event;
    if (event.ParseFromArray(data, size))
    {
        qDebug() << "Sample" << event.project().c_str();
        auto manager = Manager::instance();
        auto project = manager->projects->get(event.project().c_str());
        if (project != nullptr)
        {
            auto p = qobject_cast<kit::Project*>(project);
            p->set_remain(event.remain());
        }
    }
}
