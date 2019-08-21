#include "fileOperations.h"
#include "fileManager.h"
#include <QFile>

TCP_REQUEST(machine, DownloadFile)
{
    qDebug()<<"Build DownloadFile Request";    
    auto ret = new comm::machine::DownloadFile::Request();
    ret->set_name(name.toStdString());
    return ret;
}

TCP_REPLY(machine, DownloadFile)
{
    qDebug()<<"Process DownloadFile Reply";
    if (reply.type() == comm::OK)
    {
        comm::machine::DownloadFile::Reply r = reply.GetExtension(comm::machine::DownloadFile::Reply::EXT);
        auto manager = file::Manager::instance();
        if(r.has_data())
        {
            //Add image
            QString filepath = manager->location() + "/" + name;

            if (QFile::exists(filepath)) return 0;
            auto file = new QFile(filepath);
            if (file->open(QFile::WriteOnly))
            {
                file->write(r.data().data(),r.data().size());
                file->close();
            }
            file->deleteLater();
        }
    }
    return reply.type();
}
