#ifndef TCP_OPERATION_H
#define TCP_OPERATION_H

#include <glog/logging.h>
#include "../operation.h"
#include "connection.h"
#include "comm.pb.h"

/*service*/
#define TCP_SERVICE_REG(module, name) core::Engine::instance()->registerOperation(comm::module::name::Reply::kEXTFieldNumber, boost::make_shared<core::OperationCreator<module::name##Service>>());

#define TCP_SERVICE(module, name)\
namespace module\
{\
class name##Service: public tcp::Service<comm::module::name>\
{\
public:\
    virtual ~name##Service(){}\
    name##Service(){}\
    int execute(const comm::module::name::Request& request, comm::module::name::Reply& reply);\
};}

#define SERVICE_EXECUTE(module, name) int module::name##Service::execute(const comm::module::name::Request& request, comm::module::name::Reply& reply)

/*operation*/
#define TCP_OP(module,name)\
namespace module {\
class name##Operation: public tcp::Operation<comm::bs::name>, public boost::enable_shared_from_this<name##Operation>\
{\
public:\
    static boost::shared_ptr<name##Operation> create(){return boost::shared_ptr<name##Operation>(new name##Operation());}\
    void send(){tcp::Controller::instance()->sendRequest(comm::bs::name::Reply::kEXTFieldNumber,request_,shared_from_this());}\
    void execute(const comm::bs::name::Reply& reply);\
private:\
    name##Operation(){}

#define END_OP };}

#define OP_EXECUTE(module, name) void module::name##Operation::execute(const comm::bs::name::Reply& reply)

/*event*/
#define TCP_EVENT(module, name)\
namespace module {\
class name##Event: public tcp::Event<comm::bs::name>, public boost::enable_shared_from_this<name##Event>\
{\
public:\
    virtual ~name##Event(){}\
    void execute(const comm::bs::name& event);\
};}
#define TCP_EVENT_REG(module, name)   core::Engine::instance()->registerOperation(comm::module::name::kEXTFieldNumber, boost::make_shared<core::OperationCreator<module::name##Event>>());\

#define EVENT_EXECUTE(module, name) void module::name##Event::execute(const comm::bs::name& event)

namespace  tcp
{

class BaseOperation: public core::Operation
{
public:
    typedef boost::shared_ptr<BaseOperation> pointer;
    void setContext(uint64_t time, tcp::Connection::pointer connection, ReadMessage::pointer message)
    {
        this->connection_ = connection;
        this->message_ = message;
        this->time_ = time;
    }
protected:
    tcp::Connection::pointer connection_;
    tcp::ReadMessage::pointer message_;
    uint64_t time_;
};

template <class S>
class Service: public BaseOperation
{
public:
    Service()
    {

    }
    void run()
    {
        comm::Reply r;
        typename S::Request request;
        typename S::Reply reply;
        if (request.ParseFromArray(message_->data(), message_->size()))
        {
            auto type = execute(request, reply);
            r.set_type(type);
            if (r.type()==0 && reply.IsInitialized())
            {
                r.MutableExtension(S::Reply::EXT)->CopyFrom(reply);
            }
            else
            {
                DLOG(INFO)<<"Not Initialize";
            }
        }
        else r.set_type(comm::ErrorType::REQUEST_INVALID);
        connection_->sendMessage(message_->id(), 0, r);
    }
    virtual int execute(const typename S::Request& request, typename S::Reply& reply) = 0;
};

template <class E>
class Event: public BaseOperation
{
public:
    Event(){}
    void run()
    {
        E ev;
        if (ev.ParseFromArray(message_->data(), message_->size())) execute(ev);
        else DLOG(INFO)<<"Error in parse event";
    }
    virtual void execute(const E& event) = 0;
};

template <class O>
class Operation: public BaseOperation
{
public:
    Operation(){}
    void run()
    {
        comm::Reply r;
        if (r.ParseFromArray(message_->data(), message_->size()))
        {
            auto reply = r.GetExtension(O::Reply::EXT);
            execute(reply);
        }
        else DLOG(INFO)<<"Error in parse reply";
    }

    virtual void execute(const typename O::Reply& reply) = 0;
    typename O::Request& request(){return request_;}
protected:
    typename O::Request request_;
};

}

#endif // TCP_OPERATION_H
