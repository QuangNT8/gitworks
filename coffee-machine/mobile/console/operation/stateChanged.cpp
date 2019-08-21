#include "operations.h"
#include "manager.h"

EVENT_EXECUTE(machine, StateChanged)
{
    qDebug()<<"Event execute StateChanged";
    comm::machine::StateChanged event;
    if (event.ParseFromArray(data, size))
    {
        auto manager = console::Manager::instance();

        if (event.has_leftmotors())
        {
            manager->get_leftMotor()->set_position(event.leftmotors().position());
        }

        if (event.has_rightmotors())
        {
            manager->get_rightMotor()->set_position(event.rightmotors().position());
        }
        if(event.has_debugmode())
        {
            //Todo

            QString message =  "Change to " +QString(event.debugmode() ? "debug" : "normal") + " mode!";
            manager->sendLog("D",message);
            manager->set_debugMode(event.debugmode());
        }
        if(event.has_drinkmadeondoor())
        {
            //Todo
            manager->sendLog("D","Drink make door " +event.drinkmadeondoor());
        }
        if(event.has_machineready())
        {
            manager->set_machineReady(event.machineready());
            manager->sendLog("D","Machine " +QString(event.machineready() ? "ready" : "not ready"));
        }
        for (auto& t: event.tanks())
        {
            for (auto it : manager->tanks->data())
            {
                auto tank = qobject_cast<console::Tank*>(it);
                if (tank->get_index() == t.index())
                {
                    tank->set_volume(t.volume());
                    tank->set_liquidid(t.liquidid());
                    if (t.has_pouring()) tank->set_pouring(t.pouring());

                    auto liquid = manager->liquids->get(t.liquidid());
                    QString message = "State change: tank " + QString::number(tank->get_index()) + " liquid "
                            + (liquid == nullptr ? "id " + QString::number(t.liquidid()) : qobject_cast<console::Liquid*>(liquid)->get_name())
                            + " with " + QString::number(t.volume()) +"ml - valve " + (t.pouring() ? "close" : "open");
                    manager->sendLog("D", message);
                    break;
                }
            }
        }
    }
}
