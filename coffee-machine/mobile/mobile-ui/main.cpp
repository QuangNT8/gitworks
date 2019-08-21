#include <QGuiApplication>
#include <QApplication>
#include <QFont>
#include <QQuickStyle>
#include "flaticon/flaticon.h"
#include "engine.h"
#include "views.h"
#include "tcp/tcpController.h"
#include "QFont"
#include <manager.h>

#include "include/QZXing/QZXing.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QFont font("Cantarell", 11);
    app.setFont(font);

    QZXing::registerQMLTypes();

    auto engine = core::Engine::instance();
    QObject::connect(&app, &QGuiApplication::aboutToQuit, engine, &core::Engine::stop);

    engine->init();
    COMPONENT_REG(icon, Flaticon);
    COMPONENT_REG(home, Manager);

    engine->start();
    return app.exec();
}
