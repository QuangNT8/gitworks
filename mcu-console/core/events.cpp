#include "events.h"

void core::Events::init()
{
    Views::instance()->registerComponent("Events", this);
}

void core::Events::registerOperation(QString type, Creator<QObject>* creator)
{
    operations_[type] = creator;
}

void core::Events::registerEvent(int type, Event* event)
{
    events_[type] = event;
}

core::Event* core::Events::getEvent(int type)
{
    auto it = events_.find(type);
    if (it!=events_.end())
    {
        return  it.value();
    }
    qDebug()<<"No Command type:"<<type;
    return nullptr;
}

QObject* core::Events::create(QString type)
{
    auto it = operations_.find(type);
    if (it!=operations_.end())
    {
        return  it.value()->create();
    }
    qDebug()<<"No operation type:"<<type;
    return new Operation;
}

void core::EventOperation::run()
{
    auto event = Events::instance()->getEvent(type_);
    if (event != nullptr) event->execute(data_, size_);
    finish();
}
