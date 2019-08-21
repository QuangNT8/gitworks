#include "operations.h"
#include <QGuiApplication>
#include <views.h>
EVENT_EXECUTE(machine, LoggedOut)
{
    qDebug()<<"Event execute LoggedOut";
    core::Views::instance()->showView("qrc:/home/SplashView.qml");
    emit console::Manager::instance()->loggedOut();
}
