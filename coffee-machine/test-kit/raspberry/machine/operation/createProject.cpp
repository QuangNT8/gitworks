#include "operations.h"
#include "manager.h"
#include "utils/utils.h"
#include <fstream>

SERVICE_EXECUTE(kit, CreateProject)
{
    DLOG(INFO)<< "Execute request CreateProject";
    auto manager = kit::Manager::instance();

    data::kit::Project project;
    project.set_id(utils::generateUUID());
    project.set_name(request.name());
    project.set_file(project.id()+".pb");
    project.set_total(request.total());
    project.set_remain(request.total());
    project.set_date(utils::currentTime());
    project.set_tanksize(request.tanksize());

    manager->projects().add_projects()->CopyFrom(project);
    manager->save();
    std::fstream file;
    file.open(project.file(),std::ios::out);
    if(!file)
    {
        DLOG(INFO)<< "Error in creating file!";

        return comm::kit::CreateProject::Error::CreateProject_Error_FILE_CREATE_ERROR;
    }
    file.close();
    reply.mutable_project()->CopyFrom(project);
    return 0;
}
