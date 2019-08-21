#ifndef CORE_EVENTS_H
#define CORE_EVENTS_H

#include <QMap>
#include "views.h"
#include "component.h"
#include "operation.h"

namespace core
{
    template <class T>
    class Creator
    {
    public:
        virtual ~Creator(){}
        virtual T* create() = 0;
    };

    template <class OperationType>
    class OperationCreator : public Creator<QObject>
    {
    public:
        QObject* create()
        {
            return new OperationType;
        }
    };

    class EventOperation: public Operation
    {
    public:
        EventOperation(int type, char* data, int size): data_(data), size_(size), type_(type){}
        virtual ~EventOperation(){if (data_!=nullptr) delete[] data_;}
        void run();
    private:
        char* data_ = nullptr;
        int size_;
        int type_;
    };

    class Event
    {
    public:
        virtual void execute(char* data, int size ) = 0;
    };
}

COMPONENT(core,Events)
    Q_OBJECT
public:
    void init();
    void registerOperation(QString type, Creator<QObject>* creator);
    void registerEvent(int type, Event* event);
    Event* getEvent(int type);
    Q_INVOKABLE QObject* create(QString type);
private:
    QMap<QString, Creator<QObject>* > operations_;
    QMap<int, Event* > events_;

COMPONENT_END


#endif // CORE_EVENTS_H
