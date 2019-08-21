#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(kit, ProjectUpdated)
{
    qDebug()<<"Event execute ProjectUpdated";
    comm::kit::ProjectUpdated event;
    if (event.ParseFromArray(data, size))
    {
        auto manager = kit::Manager::instance();
        auto p = manager->projects->get(event.project().id());
        if (p != nullptr)
        {
            auto project = qobject_cast<kit::Project*>(p);
            project->data().CopyFrom(event.project());
            manager->projects->update(project);
            qDebug() << "Update project " << project->get_name();
        }
    }
}
