#include "operations.h"
#include "manager.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

SERVICE_EXECUTE(kit, Login)
{
    DLOG(INFO)<< "Execute request Login";

    auto manager = kit::Manager::instance();
    if (manager->connection() != nullptr)
    {
        comm::kit::LoggedOut event;
        manager->pushToConsole(comm::kit::LoggedOut::kEXTFieldNumber, event);
    }
    manager->setConnection(connection_);

    for (auto&p : manager->projects().projects())
    {
        auto project = reply.add_projects();
        project->CopyFrom(p);

        data::kit::File file;

        int fd = open(project->file().c_str(), O_RDONLY);
        if (fd == -1) fd = open(("build/default/install-root/" + project->file()).c_str(), O_RDONLY);

        google::protobuf::io::FileInputStream fstream(fd);
        if (google::protobuf::TextFormat::Parse(&fstream, &file))
        {
            project->mutable_samples()->CopyFrom(file.samples());
        }
        else DLOG(INFO)<<"Error in reading data from " << project->file();
        close(fd);
    }
    if (manager->projectId() != "") reply.set_currentproject(manager->projectId());
    reply.set_state(manager->state());
    return 0;
}
