#include "operations.h"
#include "manager.h"

SERVICE_EXECUTE(machine, MoveTo)
{
    DLOG(INFO)<< "Execute request MoveTo ";

    auto manager = bj::Manager::instance();
    if (request.index() == bj::LEFT) //LEFT
    {
        manager->leftMotor()->moveTo(request.position());
    }
    else if (request.index() == bj::RIGHT) //RIGHT
    {
        manager->rightMotor()->moveTo(request.position());
    }
    return 0;
}
