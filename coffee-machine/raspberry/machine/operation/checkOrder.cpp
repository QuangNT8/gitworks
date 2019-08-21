#include "operations.h"
#include "manager.h"
#include "schedule.h"

SERVICE_EXECUTE(machine, CheckOrder)
{
    DLOG(INFO)<< "Execute request CheckOrder";

    auto manager = bj::Manager::instance();
    auto schedule = bj::Schedule::instance();

    //Check debug mode
    if(manager->debugMode()) return comm::machine::CheckOrder::Error::CheckOrder_Error_IN_DEBUG_MODE;

    std::vector<std::pair<uint64_t, double>> ingredients;
    for (auto& i: request.ingredients())
    {
        std::pair<uint64_t, double> ingredient;
        ingredient.first = i.liquidid();
        ingredient.second = i.volume();
        ingredients.push_back(ingredient);

    }

    return schedule->check(static_cast<uint8_t>(request.door()), ingredients);
}
