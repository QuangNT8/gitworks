#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "core/component.h"
#include "comm.machine.pb.h"
#include "tcp/tcpOperation.h"
#include "tcp/tcpController.h"
#include "core/file/fileManager.h"
#include "models.h"
#include "manager.h"

TCP_OP(machine, KeepAlive) Q_OBJECT
END_OP

TCP_OP(machine, Login) Q_OBJECT
END_OP

TCP_OP(machine, OpenValve) Q_OBJECT
    PROPERTY(quint32, index)
    PROPERTY(quint32, time)
    PROPERTY(double, vol)
END_OP

TCP_OP(machine, MoveTo) Q_OBJECT
    PROPERTY(quint32, index)
    PROPERTY(quint32, position)
END_OP

TCP_OP(machine, ChangeTank) Q_OBJECT
    PROPERTY(quint32, index)
    PROPERTY(quint64, liquid)
    PROPERTY(double, volume)
END_OP

TCP_OP(machine, UpdateRecipe) Q_OBJECT

    PROPERTY(QString, name)
    PROPERTY(QString, desc)
    PROPERTY(QString, image)

    PROPERTY(console::Recipe*, recipe)
    PROPERTY(QVariantList, ingredients)

END_OP

TCP_OP(machine, RemoveRecipe) Q_OBJECT
    PROPERTY(console::Recipe*, recipe)
END_OP

TCP_OP(machine, MakeDrink) Q_OBJECT
    PROPERTY(quint32, door)
    PROPERTY(quint32, volume)
    PROPERTY(QVariantList, ingredients)
END_OP

TCP_OP(machine, CheckOrder) Q_OBJECT
    PROPERTY(quint32, door)
    PROPERTY(quint32, volume)
    PROPERTY(QVariantList, ingredients)
END_OP

TCP_OP(machine, UpdateLiquid) Q_OBJECT
    PROPERTY(console::Liquid*, model)
    PROPERTY(QString, name)
    PROPERTY(QString, file)
END_OP
TCP_OP(machine, UpdateDebugMode) Q_OBJECT
    PROPERTY(bool, value)
END_OP

TCP_OP(machine, ResetMachine) Q_OBJECT
END_OP


TCP_EVENT(machine, LoggedOut)
TCP_EVENT(machine, StateChanged)
TCP_EVENT(machine, MessageReceived)

REGISTRATION_BEGIN(operations)
    OPERATION_REG(machine, KeepAlive);
    OPERATION_REG(machine, Login);
    OPERATION_REG(machine, OpenValve);
    OPERATION_REG(machine, MoveTo);
    OPERATION_REG(machine, ChangeTank);
    OPERATION_REG(machine, UpdateRecipe);
    OPERATION_REG(machine, RemoveRecipe);
    OPERATION_REG(machine, MakeDrink);
    OPERATION_REG(machine, CheckOrder);
    OPERATION_REG(machine, UpdateLiquid);
    OPERATION_REG(machine, UpdateDebugMode);
    OPERATION_REG(machine, ResetMachine);

    EVENT_REG(machine, LoggedOut);
    EVENT_REG(machine, StateChanged);
    EVENT_REG(machine, MessageReceived);
REGISTRATION_END

#endif // OPERATIONS_H
