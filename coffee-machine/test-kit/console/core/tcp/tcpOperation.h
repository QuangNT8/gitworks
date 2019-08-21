#ifndef TCP_OPERATION_H
#define TCP_OPERATION_H

#include "operation.h"
#include "comm.pb.h"
typedef google::protobuf::Message ProtoMessage;

namespace tcp
{

struct Message
{
    unsigned char header[8];
    int size;
    int type;
    char* data = nullptr;
    uint64_t id;
    int state = READ_HEADER;
    enum State{READ_HEADER=0, READ_ID, READ_DATA};
};

class Operation: public core::Operation
{
    Q_OBJECT
public:
    Operation(int protocol): protocol_(protocol){}

    virtual ~Operation()
    {
       if (reply_) delete[] reply_;
       if (request_) delete[] request_;
    }

    void run();
    virtual ProtoMessage* buildRequest() = 0;
    virtual quint32 processReply(comm::Reply& reply) = 0;
protected:
    friend class Controller;
    int protocol_;
    uint64_t id_;

    char* reply_ = nullptr;
    char* request_ = nullptr;
    int replySize_;
    int requestSize_;
};

}

#define TCP_OP(module, name)\
namespace module {\
class name: public tcp::Operation\
{\
public:\
    name(): tcp::Operation(comm::module::name::Reply::kEXTFieldNumber){}\
    virtual ~name(){}\
    ProtoMessage* buildRequest();\
    quint32 processReply(comm::Reply &reply);

#define END_OP };}

#define TCP_REQUEST(module, name) ProtoMessage* module::name::buildRequest()
#define TCP_REPLY(module, name) quint32 module::name::processReply(comm::Reply &reply)

#endif // TCPOPERATION_H
