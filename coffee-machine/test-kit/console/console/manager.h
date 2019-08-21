#ifndef MANAGER_H
#define MANAGER_H
#include "component.h"
#include "tcp/tcpController.h"
#include "models.h"
#include "config.h"

COMPONENT(kit, Manager)
Q_OBJECT
public:
    void init();
    void start();

    enum State{STOPPED = 0, LIQUID_FLUSHING, TANK_CLEANING, SAMPLING, NETWORK_BUILDING, LIQUID_REFILLING};

    PROPERTY(kit::ModelList*, projects)
    PROPERTY(QString, currentId)
    READONLY(quint32, state)

    void tcpStateChanged();
    Q_SIGNAL void logged();
    Q_SIGNAL void loggedOut();

    Q_INVOKABLE QString convertTimeByLong(quint64 time);
    void sendLog(QString msg);
    Q_SIGNAL void messageReceived(QString time, QString log);

COMPONENT_END

#endif // MANAGER_H
