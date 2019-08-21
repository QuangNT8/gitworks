#include "operations.h"
#include "manager.h"
#include "config.h"
#include <fstream>

SERVICE_EXECUTE(machine, UpdateRecipe)
{
    DLOG(INFO)<< "Execute request UpdateRecipe ";

    auto manager = bj::Manager::instance();
    auto data = manager->data().mutable_recipes();

    //Add image
    if(request.recipe().imagedata().size() > 0)
    {
        auto imgdata = request.recipe().imagedata();
        std::string filepath = TEMPORARY_PATH;
        filepath = filepath + "/" + request.recipe().image();
        std::ofstream ofs(filepath, std::ios::binary | std::ios::out);
        if (!ofs.write(imgdata.data(), imgdata.size()))
        {
            DLOG(INFO)<<"write file error";
            ofs.close();
            return comm::machine::UpdateRecipe::IMAGE_INVALID;
        }
        ofs.close();

    }

    //Add Recipe
    if(request.recipe().id() == 0)
    {
        auto recipe = manager->data().add_recipes();
        recipe->CopyFrom(request.recipe());
        recipe->set_id(manager->nextId());
        recipe->clear_imagedata();
        manager->saveData();

        reply.mutable_recipe()->CopyFrom(*recipe);
        reply.mutable_recipe()->set_imagedata(request.recipe().imagedata());
        return 0;
    }

    //Update
    for (auto recipe=data->begin();recipe!=data->end();recipe++)
    {
        if(recipe->id() == request.recipe().id())
        {
            recipe->CopyFrom(request.recipe());
            recipe->clear_imagedata();
            manager->saveData();
            reply.mutable_recipe()->CopyFrom(request.recipe());
            return 0;
        }
    }

    return comm::REQUEST_INVALID;
}
