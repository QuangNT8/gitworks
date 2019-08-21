#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <QObject>
#include <QMap>
#include <typeindex>
#include "creator.h"

namespace core
{

class Component;
class Operation;
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
    void registerOperation(QString type, Creator<QObject>* creator);
    Q_INVOKABLE QObject* createOperation(QString type);
    void init();
    void start();
    void stop();

private:
    explicit Engine(QObject *parent = 0);
    QMap<std::type_index, Component*> components_;
    QMap<QString, Creator<QObject>* > operations_;
};

}

#endif // ENGINE_H
