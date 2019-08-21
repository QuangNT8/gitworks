#ifndef MANAGER_H
#define MANAGER_H
#include "component.h"
#include "tcp/tcpController.h"
#include "models.h"
#include "config.h"

COMPONENT(console, Manager)
Q_OBJECT
    PROPERTY(console::Motor*, leftMotor)
    PROPERTY(console::Motor*, rightMotor)
    PROPERTY(console::ModelList*, tanks)
    PROPERTY(console::ModelList*, recipes)
    PROPERTY(console::ModelList*, liquids)
    PROPERTY(quint64, orderid)
    ENUM2(DataIngredientFormat){ VOLUME = 0, MIN, MAX};
    READONLY(bool, debugMode)
    READONLY(bool, machineReady)
public:
    void init();
    void start();
    void tcpStateChanged();
    void sendLog(QString tag, QString log);
    Q_INVOKABLE QString getTime();
    Q_INVOKABLE quint32 getIngredient(core::Model* liquid, core::Model* recipe, DataIngredientFormat type = VOLUME);
    Q_INVOKABLE quint64 getLiquidID(core::Model* liquid){return liquid->id();}
    Q_INVOKABLE QStringList getLiquids();
    Q_INVOKABLE quint64 getLiquidByName(QString name);
    Q_INVOKABLE QString getLiquiNamedById(quint64 id);
    Q_SIGNAL void messageReceived(QString time, QString tag, QString log);
    Q_SIGNAL void neuronNetworkLoadingFailed();
    Q_SIGNAL void logged();
    Q_SIGNAL void loggedOut();

    Q_INVOKABLE quint64 getLineLength(){return LINE_LENGTH;}
    Q_INVOKABLE quint64 getMinDistanceTwoRobot(){return MIN_DISTANCE_TWO_ROBOT;}
    Q_INVOKABLE quint64 getMaxPositionRoBot(){return MAX_POSITION_ROBOT;}

COMPONENT_END

#endif // MANAGER_H
