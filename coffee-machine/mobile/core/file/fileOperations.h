#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "core/component.h"
#include "tcp/tcpOperation.h"
#include "tcp/tcpController.h"
#include "comm.machine.pb.h"

TCP_OP(machine, DownloadFile) Q_OBJECT
    PROPERTY(QString, name)
END_OP

REGISTRATION_BEGIN(file)
    OPERATION_REG(machine, DownloadFile);
REGISTRATION_END

#endif // FILEOPERATIONS_H
