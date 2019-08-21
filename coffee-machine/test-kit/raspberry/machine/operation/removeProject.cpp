#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(kit, RemoveProject)
{
    DLOG(INFO)<< "Execute request RemoveProject";
    auto manager = Manager::instance();
    if (manager->projectId() == request.id() && !manager->ready())
        return comm::kit::RemoveProject::IS_RUNNING;

    for (int i = 0; i < manager->projects().projects_size(); i++)
    {
        if (manager->projects().projects(i).id() == request.id())
        {
            manager->projects().mutable_projects()->DeleteSubrange(i, 1);
            manager->save();
            break;
        }
    }

    return 0;
}
