#include "manager.h"
#include "core/views.h"
#include "operation/operations.h"
#include <QFile>
void kit::Manager::init()
{
    core::Views::instance()->registerComponent("Kit", this);
    connect(tcp::Controller::instance(),&tcp::Controller::stateChanged,this, &Manager::tcpStateChanged);
    REGISTRATION(operations)
    core::Views::instance()->setWidth(1366);
    core::Views::instance()->setHeight(768);    

    state = STOPPED;
    currentId  = "";
    projects = new kit::ModelList(this);
}

void kit::Manager::start()
{
     tcp::Controller::instance()->open(KIT_IP, SERVER_PORT);
}

void kit::Manager::tcpStateChanged()
{
    auto state = tcp::Controller::instance()->state;
    switch (state)
    {
        case tcp::Controller::CONNECTED:
        {
            auto op = new kit::Login();
            op->fire();
            break;
        }
        case tcp::Controller::DISCONNECTED:
        {
            break;
        }
    }
}

QString kit::Manager::convertTimeByLong(quint64 time)
{
    QDateTime absDateTime = QDateTime::fromTime_t(time/1000, Qt::LocalTime);
    return absDateTime.toString("hh:mm:ss dd/MM/yyyy");
}

void kit::Manager::sendLog(QString msg)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    emit this->messageReceived(time, msg);
}
