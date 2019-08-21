#ifndef TCP_CONTROLLER_H
#define TCP_CONTROLLER_H
#include "engine.h"
#include <unordered_map>
#include <boost/function.hpp>
#include "connection.h"
#include "operation.h"

COMPONENT(tcp, Controller)
public:
    enum State{OFFLINE=0, CONNECTING, CONNECTED, ONLINE};
    Controller();

    void start();
    void stop();

    void open(std::string host, int port);
    void close();

    tcp::BaseOperation::pointer takeOperation(uint64_t id);
    void clearOperations();
    void sendRequest(uint16_t type, const google::protobuf::Message& request, tcp::BaseOperation::pointer op);
    void setConnectedHandler(boost::function<void()> handler){connectedHandler_ = handler;}
private:
    boost::function<void()> connectedHandler_;
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::acceptor acceptor_;
    tcp::Connection::pointer connection_;
    int state_ = OFFLINE;

    uint64_t nextId_ = 100;
    boost::mutex opMutex_;
    std::unordered_map<uint64_t, tcp::BaseOperation::pointer> operations_;

    void startConnect_(const boost::system::error_code& err,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    void handleConnect_(const boost::system::error_code& err,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    void startAccept_();
    void handleAccept_(tcp::Connection::pointer connection, const boost::system::error_code& error);

COMPONENT_END

#endif // TCP_CONTROLLER_H


