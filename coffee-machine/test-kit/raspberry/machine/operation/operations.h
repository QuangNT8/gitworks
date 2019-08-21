#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "tcp/operation.h"
#include "tcp/controller.h"
#include "comm.kit.pb.h"
#include "engine.h"

TCP_SERVICE(kit, Login)
TCP_SERVICE(kit, CreateProject)
TCP_SERVICE(kit, RemoveProject)
TCP_SERVICE(kit, BuildNetwork)
TCP_SERVICE(kit, StartProject)
TCP_SERVICE(kit, StopProject)
TCP_SERVICE(kit, CleanTank)

TCP_SERVICE(kit, OpenValue)
TCP_SERVICE(kit, FlushTank)
TCP_SERVICE(kit, ReFillTank)

REGISTRATION_BEGIN(operations)
    TCP_SERVICE_REG(kit, Login)
    TCP_SERVICE_REG(kit, CreateProject)
    TCP_SERVICE_REG(kit, RemoveProject)
    TCP_SERVICE_REG(kit, BuildNetwork)
    TCP_SERVICE_REG(kit, StartProject)
    TCP_SERVICE_REG(kit, StopProject)
    TCP_SERVICE_REG(kit, CleanTank)

    TCP_SERVICE_REG(kit, OpenValue)
    TCP_SERVICE_REG(kit, FlushTank)
    TCP_SERVICE_REG(kit, ReFillTank)
REGISTRATION_END

#endif // OPERATIONS_H
