#include "engine.h"
#include <QDebug>
#include <QThreadPool>
#include "component.h"

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

void Engine::start()
{
    foreach (Component* c, components_) c->init();
    foreach (Component* c, components_) c->start();
}

void Engine::stop()
{
    foreach (Component* c, components_) c->stop();
}

}
