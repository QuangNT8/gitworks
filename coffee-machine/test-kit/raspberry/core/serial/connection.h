#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H

#define HEADER_INDICATOR	0xFE
#define FOOTER_INDICATOR	0xFD
#define MAX_PACKET_LENGTH	250

#include "engine.h"
#include <boost/asio/serial_port.hpp>

namespace serial
{

class WriteMessage
{
public:
    typedef boost::shared_ptr<WriteMessage> pointer;
    WriteMessage(uint16_t type, uint8_t length, const uint8_t* data)
    {
        size_ = length + 5;
        data_ = new uint8_t[size_];
        int index = 0;
        data_[index++] = HEADER_INDICATOR;
        data_[index++] = length;
        data_[index++] = ((type >> 8)& 0xFF);
        data_[index++] = (type & 0xFF);
        for (int i =0; i< length;i++)
        {
            data_[index++] = data[i];
        }
        data_[index++] = FOOTER_INDICATOR;
    }
    virtual ~WriteMessage()
    {
        if (data_ != nullptr) delete [] data_;
    }
    char* data() {return (char*)data_;}
    uint16_t size(){return size_;}
private:
    uint8_t* data_ = nullptr;
    uint16_t size_;
};

class Connection: public boost::enable_shared_from_this<Connection>
{
public:
    typedef boost::shared_ptr<Connection> pointer;
    Connection(std::string port);
    void sendMessage(uint16_t type, uint8_t length, const uint8_t* data);
    void start();
    void stop();
private:
    boost::asio::serial_port port_;
    boost::asio::io_service::strand strand_;

    void write_(WriteMessage::pointer m);

    void handleReadIndicator_(const boost::system::error_code& e);
    void handleReadHeader_(const boost::system::error_code& e);
    void handleReadData_(const boost::system::error_code& e);
    void handleWrite_(const boost::system::error_code& e);

    uint8_t indicator_;
    uint8_t header_[3];
    uint16_t type_;
    uint8_t length_;
    uint8_t data_[256];
    std::string portName_;

    std::deque<WriteMessage::pointer> messages_;
};

}

#endif // CONNECTION_H
