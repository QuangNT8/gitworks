#include "operations.h"
#include <QFile>
#include <QUuid>

TCP_REQUEST(machine, UpdateRecipe)
{
    qDebug()<<"Build UpdateRecipe Request";
    auto ret = new comm::machine::UpdateRecipe::Request();
    auto manager = console::Manager::instance();

    data::bj::Recipe data;
    if (recipe == nullptr) //create
    {
        data.set_id(0);
    }
    else
    {
        data.set_id(recipe->id()); //Edit
        data.set_image(recipe->get_image().toStdString());
    }

    data.set_name(name.toStdString());
    data.set_description(desc.toStdString());

    //Add ingredient
    for(auto &it : ingredients)
    {
        auto value = it.toMap();
        auto addvl = data.add_ingredients();
        addvl->set_liquidid(static_cast<uint64_t>(value.value("liquidid").toInt()));
        addvl->set_volume(static_cast<uint32_t>(value.value("volume").toInt()));
    }

    //Add image
    if((recipe == nullptr && image.length() > 0)
       || ( recipe != nullptr && image.length() > 0 && image !=recipe->get_image())
    ) //Add or Edit && image change
    {
        //Upload image
        QUrl url(image);
        auto file = new QFile(url.toLocalFile());
        if(file->size() > MAX_FILE_SIZE || file->size() == 0 || !file->open(QFile::ReadOnly))
        {
            file->deleteLater();
            manager->sendLog("E", "Upload image error: " +name );
            manager->sendLog("E", "Max image size allowed: "+QString::number(MAX_FILE_SIZE)+". Your image size: "+ QString::number(file->size()));

            return nullptr;
        }

        auto dataimg = file->read(file->size());
        auto uuid = QUuid::createUuid();
        QString filename = uuid.toString(QUuid::Id128);
        data.set_image(filename.toStdString());
        data.set_imagedata(dataimg.data(), dataimg.size());

        file->close();
        file->deleteLater();

    }
    ret->mutable_recipe()->CopyFrom(data);
    return ret;
}

TCP_REPLY(machine, UpdateRecipe)
{
    qDebug()<<"Process UpdateRecipe Reply";
    if (reply.type() == comm::OK)
    {
        comm::machine::UpdateRecipe::Reply r = reply.GetExtension(comm::machine::UpdateRecipe::Reply::EXT);
        auto manager = console::Manager::instance();

        if(r.recipe().imagedata().size() > 0)
        {
            //Add image
            QString filepath = file::Manager::instance()->location() + "/" + r.recipe().image().c_str();
            auto file = new QFile(filepath);
            bool check = false;
            if (file->open(QFile::WriteOnly))
            {
                file->write(r.recipe().imagedata().data(),r.recipe().imagedata().size());
                file->close();
                check = true;
            }
            file->deleteLater();
            if(!check) manager->sendLog("E","Write image error: " +QString::fromStdString(r.recipe().image()) );
        }

        if(recipe == nullptr) //Add recipe
        {
            manager->recipes->add(new console::Recipe(r.recipe()));
            manager->sendLog("D", "Add recipe: " +name);
        }
        else
        {
            auto t = manager->recipes->get(r.recipe().id());
            if (t != nullptr)
            {
                auto target = qobject_cast<console::Recipe*>(t);
                target->data().CopyFrom(r.recipe());
                manager->recipes->update(target);
                manager->sendLog("D", "Update recipe: " +recipe->get_name());
            }
        }

    }
    return reply.type();
}
