#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

#define COMPONENT(module, name)\
namespace module{\
class name: public core::Component{ \
public: \
    virtual ~name(){} \
    static boost::shared_ptr<name> instance()\
    { \
        static auto _instance = core::Engine::instance()->getComponent<name>();\
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
class Component
{
public:
    explicit Component(){}
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
