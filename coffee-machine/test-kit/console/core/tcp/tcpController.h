#ifndef TCPSERVER_H
#define TCPSERVER_H

#define PACKET_HEADER_SIZE		8
#define PACKET_MAGIC			0xEE

#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include <unordered_map>
#include "component.h"
#include "operation.h"
#include "tcpOperation.h"
#include "config.h"
#include "define.h"

namespace tcp
{
    class Event: public core::Operation
    {
        Q_OBJECT
    public:
        Event(){}

        virtual ~Event()
        {
            if (data_!=nullptr) delete[] data_;
        }

        void setData(char* data, int size)
        {
            data_ = data;
            size_ = size;
        }

        virtual void execute(char* data, int size) = 0;
        void run()
        {
            execute(data_, size_);
            finish(0);
        }
    private:
        char* data_ = nullptr;
        int size_;
    };

    template <class EventType>
    class EventCreator : public core::Creator<Event>
    {
    public:
        Event* create()
        {
            return new EventType;
        }
    };
}

#define TCP_EVENT(module, name)\
namespace module {\
class name: public tcp::Event\
{\
public:\
    virtual ~name(){}\
    void execute(char* data, int size);\
};}

#define EVENT_REG(module, name)\
{\
    tcp::Controller::instance()->registerEvent(comm::module::name::kEXTFieldNumber, new tcp::EventCreator<module::name>);\
}

#define EVENT_EXECUTE(module, name) void module::name::execute(char* data, int size)

COMPONENT(tcp, Controller)
    Q_OBJECT
    ENUM2(State){ DISCONNECTED = 0, CONNECTING, CONNECTED, DISCONNECTING};
    PROPERTY(int, state)
    Q_SIGNAL void sendMessage(Operation*);
public:
    void init();
    void start();
    void stop();
    void registerEvent(uint16_t type, core::Creator<Event>* creator);
    void open(QString host, int port);
    void close();
    uint64_t nextId();
    uint64_t lastReceived(){return lastReceived_;}
private:

    Event* create_(uint16_t type);
    void readyRead_();
    void sendMessage_(Operation* op);

    QTcpSocket* socket_;
    Message message_;
    quint64 sequence_ = 1000;
    quint64 lastReceived_ = 0;
    QMutex mutex_;

    std::unordered_map<uint64_t, Operation*> operations_;
    QMap<uint16_t, core::Creator<Event>* > events_;

COMPONENT_END

#endif // TCPSERVER_H
