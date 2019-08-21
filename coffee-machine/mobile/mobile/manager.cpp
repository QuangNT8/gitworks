#include "manager.h"
#include "core/views.h"
#include "operation/operations.h"

void home::Manager::init()
{
    core::Views::instance()->registerComponent("Home", this);
    connect(tcp::Controller::instance(),&tcp::Controller::stateChanged,this, &Manager::tcpStateChanged);

    REGISTRATION(operations)

    recipes = new home::ModelList(this);
    liquids = new home::ModelList(this);
    data::bj::Session s;
    s.set_id(0);
    session = new home::Session(s);
}

void home::Manager::start()
{
    tcp::Controller::instance()->open(SERVER_IP, SERVER_PORT);
}

void home::Manager::tcpStateChanged()
{
    auto state = tcp::Controller::instance()->state;
    switch (state)
    {
        case tcp::Controller::CONNECTED:
        {
            auto login = new machine::Login();
            login->fire();
            break;
        }
        case tcp::Controller::DISCONNECTED:
        {
            break;
        }
    }
}
quint32 home::Manager::getIngredient(core::Model* liquid, core::Model* recipe)
{
    auto r = Manager::instance()->recipes->get(recipe->id());
    if(r != nullptr)
    {
        auto rp = qobject_cast<home::Recipe*>(r);
        for(auto& l: rp->data().ingredients())
            if(l.liquidid() == liquid->id()) return l.volume();
    }
    return 0;
}
