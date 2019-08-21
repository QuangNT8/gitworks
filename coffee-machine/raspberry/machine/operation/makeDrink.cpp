#include "operations.h"
#include "schedule.h"
#include "manager.h"

SERVICE_EXECUTE(machine, MakeDrink)
{
    DLOG(INFO)<< "Execute request MakeDrink";

    auto schedule = bj::Schedule::instance();

    //Check debug mode
    if(bj::Manager::instance()->debugMode()) return comm::machine::MakeDrink::Error::MakeDrink_Error_IN_DEBUG_MODE;

    std::vector<std::pair<uint64_t, double>> ingredients;
    for (auto& i: request.ingredients())
    {
        std::pair<uint64_t, double> ingredient;
        ingredient.first = i.liquidid();
        ingredient.second = i.volume();
        ingredients.push_back(ingredient);

    }
    if(request.door() == bj::LEFT) bj::Manager::instance()->ingredientsLeft = ingredients;
    else bj::Manager::instance()->ingredientsRight = ingredients;
    auto loop = (request.has_loop()?request.loop():false);
    return schedule->make(connection_->getSessionId(), static_cast<uint8_t>(request.door()), ingredients, loop);
}
