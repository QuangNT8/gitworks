#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, Login)
{
    DLOG(INFO)<< "Execute request Login";

    auto manager = bj::Manager::instance();

    if (request.type() == 1) //for console
    {
        comm::machine::LoggedOut event;
        if (manager->getConsoleConnection() != nullptr)
            manager->pushToConsole(comm::machine::LoggedOut::kEXTFieldNumber, event);

        connection_->setSessionId(0);
        manager->setConsoleConnection(connection_); //bind new connection
    }
    else
    {
        auto id = manager->sessionId();
        connection_->setSessionId(id);
        manager->setSession(id, connection_);
        reply.mutable_session()->set_id(id);
        reply.mutable_session()->set_host(SERVER_IP);
        reply.mutable_session()->set_port(SERVER_PORT);
    }

    reply.set_debugmode(manager->debugMode());
    reply.set_machineready(manager->machineReady());

    for (auto& l: manager->data().liquids()) reply.add_liquids()->CopyFrom(l);
    for (auto& r: manager->data().recipes()) reply.add_recipes()->CopyFrom(r);

    for (uint32_t index = 0; index < TANK_COUNT; index++)
    {
        auto t = manager->getTank(index);
        if (t != nullptr)
        {
            auto tank = reply.add_tanks();
            tank->set_index(index);
            tank->set_volume(t->volume());
            tank->set_liquidid(t->liquid());
            tank->set_pouring(t->closed());
        }
    }

    reply.mutable_leftmotors()->set_position(manager->leftMotor()->position());
    reply.mutable_rightmotors()->set_position(manager->rightMotor()->position());
    return 0;
}
