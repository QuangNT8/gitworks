#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "tcp/operation.h"
#include "tcp/controller.h"
#include "comm.machine.pb.h"
#include "engine.h"

TCP_SERVICE(machine, KeepAlive)
TCP_SERVICE(machine, Login)
TCP_SERVICE(machine, OpenValve)
TCP_SERVICE(machine, MoveTo)
TCP_SERVICE(machine, ChangeTank)
TCP_SERVICE(machine, UpdateRecipe)
TCP_SERVICE(machine, RemoveRecipe)
TCP_SERVICE(machine, CheckOrder)
TCP_SERVICE(machine, MakeDrink)
TCP_SERVICE(machine, UpdateLiquid)
TCP_SERVICE(machine, DownloadFile)
TCP_SERVICE(machine, UpdateDebugMode)
TCP_SERVICE(machine, ResetMachine)


REGISTRATION_BEGIN(operations)
    TCP_SERVICE_REG(machine, KeepAlive)
    TCP_SERVICE_REG(machine, Login)
    TCP_SERVICE_REG(machine, OpenValve)
    TCP_SERVICE_REG(machine, MoveTo)
    TCP_SERVICE_REG(machine, ChangeTank)
    TCP_SERVICE_REG(machine, UpdateRecipe)
    TCP_SERVICE_REG(machine, RemoveRecipe)
    TCP_SERVICE_REG(machine, CheckOrder)
    TCP_SERVICE_REG(machine, MakeDrink)
    TCP_SERVICE_REG(machine, UpdateLiquid)
    TCP_SERVICE_REG(machine, DownloadFile)
    TCP_SERVICE_REG(machine, UpdateDebugMode)
    TCP_SERVICE_REG(machine, ResetMachine)
REGISTRATION_END

#endif // OPERATIONS_H
