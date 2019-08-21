#include "engine.h"
#include <QDebug>
#include <QThreadPool>
#include "component.h"
#include "operation.h"
#include "views.h"
#include "tcp/tcpController.h"
#include <QRegularExpression>

namespace core {

Engine::Engine(QObject *parent) : QObject(parent)
{

}

void Engine::registerComponent(Component* component)
{
    auto t = std::type_index(typeid(*component));
    components_[t] = component;
    component->engine_ = this;
}

Component* Engine::getComponent(std::type_index type)
{
    auto it = components_.find(type);
    if (it==components_.end()) return nullptr;
    return it.value();
}

void Engine::init()
{
    /*Register core components*/
    this->registerComponent(new core::Views());
    this->registerComponent(new tcp::Controller());
    core::Views::instance()->registerComponent("Engine", this);
}
void Engine::start()
{
    foreach (Component* c, components_) c->init();
    foreach (Component* c, components_) c->start();
}

void Engine::stop()
{
    foreach (Component* c, components_) c->stop();
}

void Engine::registerOperation(QString type, Creator<QObject>* creator)
{
    operations_[type] = creator;
}

QObject* Engine::createOperation(QString type)
{
    auto it = operations_.find(type);
    if (it!=operations_.end())
    {
        return  it.value()->create();
    }
    qDebug()<<"Engine No operation type:"<<type;
    return new Operation;
}

}
