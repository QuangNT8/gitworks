#include <QApplication>
#include "core/engine.h"
#include "core/views.h"
#include "serial/controller.h"
#include "console.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    auto engine = core::Engine::instance();
    QObject::connect(&app, &QGuiApplication::aboutToQuit, engine, &core::Engine::stop);

    /*REGISTER COMPONENTS*/
    COMPONENT_REG(core, Views);
    COMPONENT_REG(serial, Controller);
    COMPONENT_REG(console, Controller);

    engine->start();
    return app.exec();
}
