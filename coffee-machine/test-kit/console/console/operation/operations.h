#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "core/component.h"
#include "tcp/tcpOperation.h"
#include "tcp/tcpController.h"
#include "comm.kit.pb.h"
#include "models.h"
#include "manager.h"

TCP_OP(kit, Login) Q_OBJECT
END_OP

TCP_OP(kit, CreateProject) Q_OBJECT
    PROPERTY(QString, name)
    PROPERTY(quint32, total)
    PROPERTY(double, tankSize)
END_OP

TCP_OP(kit, RemoveProject) Q_OBJECT
    PROPERTY(kit::Project*, project)
END_OP

TCP_OP(kit, BuildNetwork) Q_OBJECT
    PROPERTY(kit::Project*, project)
END_OP

TCP_OP(kit, StartProject) Q_OBJECT
    PROPERTY(kit::Project*, project)
END_OP

TCP_OP(kit, StopProject) Q_OBJECT
END_OP

TCP_OP(kit, CleanTank) Q_OBJECT
END_OP

TCP_OP(kit, OpenValue) Q_OBJECT
    PROPERTY(quint32, value)
    PROPERTY(kit::Project*, project)
END_OP

TCP_OP(kit, FlushTank) Q_OBJECT
END_OP

TCP_OP(kit, ReFillTank) Q_OBJECT
END_OP

TCP_EVENT(kit, LoggedOut)
TCP_EVENT(kit, SampleCreated)
TCP_EVENT(kit, ProjectUpdated)
TCP_EVENT(kit, MessageReceived)
TCP_EVENT(kit, StateChanged)

REGISTRATION_BEGIN(operations)
    OPERATION_REG(kit, Login);
    OPERATION_REG(kit, CreateProject);
    OPERATION_REG(kit, RemoveProject);
    OPERATION_REG(kit, BuildNetwork);
    OPERATION_REG(kit, StartProject);
    OPERATION_REG(kit, StopProject);
    OPERATION_REG(kit, CleanTank);

    OPERATION_REG(kit, OpenValue);
    OPERATION_REG(kit, FlushTank);
    OPERATION_REG(kit, ReFillTank);

    EVENT_REG(kit, LoggedOut);
    EVENT_REG(kit, SampleCreated);
    EVENT_REG(kit, ProjectUpdated);
    EVENT_REG(kit, MessageReceived);
    EVENT_REG(kit, StateChanged);
REGISTRATION_END

#endif // OPERATIONS_H
