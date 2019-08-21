#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <QObject>
#include <QMap>
#include <typeindex>

namespace core
{

class Component;
class Engine : public QObject
{
    Q_OBJECT
public:
    static Engine* instance()
    {
        static Engine* e = new Engine();
        return e;
    }

    void registerComponent(Component* component);
    Component* getComponent(std::type_index type);
    void start();
    void stop();
private:
    explicit Engine(QObject *parent = 0);
    QMap<std::type_index, Component*> components_;
};

}

#endif // ENGINE_H
