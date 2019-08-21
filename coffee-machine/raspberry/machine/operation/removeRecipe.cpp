#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, RemoveRecipe)
{
    DLOG(INFO)<< "Execute request RemoveRecipe ";

    auto manager = bj::Manager::instance();

    auto rps = manager->data().recipes();
    auto recipes = manager->data().mutable_recipes();

    //Remove all
    recipes->Clear();
    for (int i = 0; i < rps.size(); i++)
    {
        if(rps.Get(i).id() != request.id())
        {
            recipes->Add()->CopyFrom(rps.Get(i));
        }
    }
    manager->saveData();
    return 0;
}
