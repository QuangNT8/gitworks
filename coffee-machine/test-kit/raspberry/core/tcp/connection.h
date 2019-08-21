#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H
#include <deque>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "message.h"

namespace tcp
{

class Connection: public boost::enable_shared_from_this<Connection>
{
public:
    typedef boost::shared_ptr<Connection> pointer;
    typedef boost::weak_ptr<Connection> weak_pointer;
    static pointer create(boost::asio::io_service& service);
    boost::asio::ip::tcp::socket& socket();

    void start();
    void sendMessage(uint64_t id, uint16_t type, const google::protobuf::Message& m);

    uint64_t getSessionId(){return sessionId_;}
    void setSessionId(uint64_t id){this->sessionId_= id;}

    std::string ip(){return socket_.remote_endpoint().address().to_string();}
    uint64_t lastTime(){return lastTime_;}
private:
    void write_(WriteMessage::pointer m);
    Connection(boost::asio::io_service& service);
    void handleReadHeader_(const boost::system::error_code& e);
    void handleReadId_(const boost::system::error_code& e);
    void handleReadData_(const boost::system::error_code& e);
    void handleWrite_(const boost::system::error_code& e);

    boost::asio::io_service::strand strand_;
    boost::asio::ip::tcp::socket socket_;

    ReadMessage::pointer readMessage_;
    char headerData_[16];
    uint64_t sessionId_ = 0;
    uint64_t lastTime_ = 0;

    std::deque<WriteMessage::pointer> messages_;
};

}

#endif // CONNECTION_H
