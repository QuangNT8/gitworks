#ifndef TCP_MESSAGE_H
#define TCP_MESSAGE_H

#define PACKET_MAGIC    0xEE

#include <boost/shared_ptr.hpp>
#include "../commons/comm.pb.h"
#include <glog/logging.h>

namespace tcp
{

class ReadMessage
{
public:
    enum Protocol{REQUEST=0, REPLY, EVENT};
    typedef boost::shared_ptr<ReadMessage> pointer;
    ReadMessage(char* headerData)
    {
        uint8_t * header = (uint8_t*) headerData;

        if (header[0] != PACKET_MAGIC || header[1] != PACKET_MAGIC)
        {
            DLOG(INFO) << "Magic bytes not match";
            valid_ = false;
        }
        else
        {
            valid_ = true;
            size_ = (header[2] << 16) + (header[3]<<8) + header[4]; // 3 bytes for size
            type_ = (header[5] << 8) + header[6];//2 bytes for type

            if ((header[7] & 0x02) == 2)
            {
                size_ -= 16;
                if (type_ == 0) protocol_ = REPLY;
                else protocol_ = REQUEST;
            }
            else
            {
                id_ = 0;
                protocol_ = EVENT;
                size_ -= 8;
            }

            data_ = new char[size_];
        }
    }
    ~ReadMessage(){if (data_) delete data_;}
    char* data(){return data_;}
    uint32_t size(){return size_;}
    uint64_t id(){return id_;}
    uint32_t type(){return type_;}
    bool valid(){return valid_;}
    int protocol(){return protocol_;}
    void setId(uint64_t id){this->id_ = id;}
private:
    char* data_ = nullptr;
    uint64_t id_;
    uint32_t type_;
    uint32_t size_;
    bool valid_;
    int protocol_ = 0;
};

class WriteMessage
{
public:
    typedef boost::shared_ptr<WriteMessage> pointer;
    WriteMessage(uint64_t id, uint16_t type, const google::protobuf::Message& message)
    {
        uint32_t s = message.ByteSize();
        if (id>0) size_ = s+16;
        else size_ = s+8;

        data_ = new uint8_t[size_];

        data_[0] = PACKET_MAGIC;
        data_[1] = PACKET_MAGIC;

        data_[2] = ((size_ >> 16) & 0xFF);
        data_[3] = ((size_ >> 8) & 0xFF);
        data_[4] = (size_ & 0xFF);

        data_[5] = ((type >> 8) & 0xFF);
        data_[6] = (type & 0xFF);

        char* tmp = (char*)(data_ + 8);

        if (id>0)
        {
            data_[7] = 2;
            data_[8] = ((id >> 56) & 0xFF);
            data_[9] = ((id >> 48) & 0xFF);
            data_[10] = ((id >> 40) & 0xFF);
            data_[11] = ((id >> 32) & 0xFF);
            data_[12] = ((id >> 24) & 0xFF);
            data_[13] = ((id >> 16) & 0xFF);
            data_[14] = ((id >> 8) & 0xFF);
            data_[15] = (id & 0xFF);
            tmp += 8;
        }
        else data_[7] = 0;
        valid_ = message.SerializeToArray(tmp, s);

    }
    ~WriteMessage()
    {
        if (data_) delete[] data_;
    }

    char* data(){return (char*)data_;}
    uint32_t size(){return size_;}
    bool valid(){return valid_;}
private:
    bool valid_;
    uint8_t* data_ = nullptr;
    uint32_t size_;
};

}

#endif // TCP_MESSAGE_H
