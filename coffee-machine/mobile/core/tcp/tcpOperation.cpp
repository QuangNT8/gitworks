#include "tcpOperation.h"
#include "tcpController.h"

void tcp::Operation::run()
{
    if (request_==nullptr)
    {
        auto controller = tcp::Controller::instance();
        auto message = buildRequest();
        if (message == nullptr)
        {
            finish(0);
            return;
        }

        std::string data = message->SerializeAsString();
        auto id = controller->nextId();
        int size = data.size() + PACKET_HEADER_SIZE + 8;
        int type = protocol_;

        unsigned char* buf = new unsigned char[size];

        buf[0] = PACKET_MAGIC;
        buf[1] = PACKET_MAGIC;

        buf[2] = ((size >> 16) & 0xFF);
        buf[3] = ((size >> 8) & 0xFF);
        buf[4] = (size & 0xFF);

        buf[5] = ((type >> 8) & 0xFF);
        buf[6] = (type & 0xFF);
        buf[7] = 2;

        buf[8] = ((id >> 56) & 0xFF);
        buf[9] = ((id >> 48) & 0xFF);
        buf[10] = ((id >> 40) & 0xFF);
        buf[11] = ((id >> 32) & 0xFF);
        buf[12] = ((id >> 24) & 0xFF);
        buf[13] = ((id >> 16) & 0xFF);
        buf[14] = ((id >> 8) & 0xFF);
        buf[15] = (id & 0xFF);

        memcpy(buf + 16, data.c_str(), data.size());
        request_ = (char*)buf;
        requestSize_ = size;
        id_ = id;
        delete message;
        emit controller->sendMessage(this);
    }
    else
    {
        comm::Reply reply;
        if (reply.ParseFromArray(reply_, replySize_))
        {
            auto code = processReply(reply);
            finish(code);
        }
        else finish(0);
    }
}

