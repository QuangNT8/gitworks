#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H
#include <QObject>
#include <typeindex>
#include <string>
#include "engine.h"

#define COMPONENT(module, name)\
namespace module{\
class name: public core::Component{ \
public: \
    name(QObject* parent=0):core::Component(parent){} \
    virtual ~name(){} \
    static name* instance(){ \
        static name* _instance = (name*)core::Engine::instance()->getComponent(std::type_index(typeid(name))); \
        return _instance; \
    }

#define COMPONENT_END };}

#define REGISTRATION_BEGIN(module)\
namespace module{\
class Registration{\
public:\
static void run(){\

#define REGISTRATION_END     }};}
#define REGISTRATION(module) module::Registration::run();

namespace core
{

class Engine;
class Component : public QObject
{
    Q_OBJECT
public:
    explicit Component(QObject *parent = 0): QObject(parent){}
    virtual ~Component(){this->release();}

    virtual void init(){}
    virtual void start(){}
    virtual void stop(){}
    virtual void release(){}
private:
    Engine* engine_;
    friend class Engine;
};

}

#endif // COMPONENT_H
