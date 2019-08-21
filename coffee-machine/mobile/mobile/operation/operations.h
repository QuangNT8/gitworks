#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "models.h"
#include "core/component.h"
#include "comm.machine.pb.h"
#include "tcp/tcpOperation.h"
#include "tcp/tcpController.h"
#include "manager.h"

TCP_OP(machine, Login) Q_OBJECT
END_OP

TCP_OP(machine, CheckOrder) Q_OBJECT
    PROPERTY(quint32, door)
    PROPERTY(quint32, volume)
    PROPERTY(QVariantList, ingredients)
END_OP

TCP_OP(machine, MakeDrink) Q_OBJECT
    PROPERTY(quint32, door)
    PROPERTY(quint32, volume)
    PROPERTY(QVariantList, ingredients)
END_OP

TCP_EVENT(machine, OrderStateChanged)

REGISTRATION_BEGIN(operations)
    OPERATION_REG(machine, Login);
    OPERATION_REG(machine, CheckOrder);
    OPERATION_REG(machine, MakeDrink);

    EVENT_REG(machine, OrderStateChanged);
REGISTRATION_END

#endif // OPERATIONS_H
