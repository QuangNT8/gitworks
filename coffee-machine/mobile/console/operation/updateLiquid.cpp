#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <sys/fcntl.h>
#include "operations.h"
#include "../raspberry/ann/neuronnetwork.h"

TCP_REQUEST(machine, UpdateLiquid)
{
    qDebug()<<"Build UpdateLiquid Request";
    auto ret = new comm::machine::UpdateLiquid::Request();
    auto manager = console::Manager::instance();
    data::bj::Liquid data;

    if (model == nullptr)
    {
        data.set_id(0);
        data.set_name(name.toStdString());
    }
    else
    {
        data.CopyFrom(model->data());
        data.set_name(name.toStdString());
    }

    if (file != nullptr && file != "")
    {
        if (file.startsWith("file:///")) file = file.right(file.length() - 7);

        data::bj::Network dt;
        int fd = open(file.toStdString().c_str(), O_RDONLY);
        google::protobuf::io::FileInputStream fstream(fd);
        if (!google::protobuf::TextFormat::Parse(&fstream, &dt))
        {
            emit manager->neuronNetworkLoadingFailed();
            emit manager->sendLog("E", "Neuron network loading failed");
            return nullptr;
        }

        data.mutable_network()->CopyFrom(dt);

        manager->sendLog("D", "Update liquid: " + name + " - neuron network loading completed");
    }
    ret->mutable_liquid()->CopyFrom(data);
    return ret;
}

TCP_REPLY(machine, UpdateLiquid)
{
    qDebug()<<"Process UpdateLiquid Reply";
    if (reply.type() == comm::OK)
    {
        comm::machine::UpdateLiquid::Reply r = reply.GetExtension(comm::machine::UpdateLiquid::Reply::EXT);
        auto manager = console::Manager::instance();
        if (model != nullptr)
        {
            auto l = manager->liquids->get(r.liquid().id());
            if (l != nullptr)
            {
                auto liquid = qobject_cast<console::Liquid*>(l);
                liquid->set_name(r.liquid().name());
                liquid->data().mutable_network()->CopyFrom(r.liquid().network());
                manager->liquids->update(liquid);
                qDebug() << "Update liquid" << r.liquid().name().c_str();
            }
        }
        else
        {
            qDebug() << "Add liquid" << r.liquid().name().c_str();
            manager->liquids->add(new console::Liquid(r.liquid()));
        }
    }
    return reply.type();
}
