#include "controller.h"
#include "../utils.h"

tcp::Controller::Controller():
    io_service_(core::Engine::instance()->getService()),
    resolver_(core::Engine::instance()->getService()),
    acceptor_(core::Engine::instance()->getService())
{

}

tcp::BaseOperation::pointer tcp::Controller::takeOperation(uint64_t id)
{
    boost::mutex::scoped_lock lock(opMutex_);
    auto it = operations_.find(id);
    if (it!=operations_.end())
    {
        auto ret = it->second;
        operations_.erase(it);
        return ret;
    }
    return nullptr;
}

void tcp::Controller::clearOperations()
{
    boost::mutex::scoped_lock lock(opMutex_);
    operations_.clear();
}

void tcp::Controller::sendRequest(uint16_t type, const google::protobuf::Message& request, tcp::BaseOperation::pointer op)
{
    boost::mutex::scoped_lock lock(opMutex_);
    auto id = nextId_++;
    operations_[id] = op;
    connection_->sendMessage(id, type, request);
}

void tcp::Controller::open(std::string host, int port)
{
    boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
        resolver_.async_resolve(query,
              boost::bind(&tcp::Controller::startConnect_, this,
              boost::asio::placeholders::error,
              boost::asio::placeholders::iterator));
}

void tcp::Controller::startConnect_(const boost::system::error_code& err,boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
        connection_ = tcp::Connection::create(io_service_);
        connection_->socket().async_connect(endpoint, boost::bind(&tcp::Controller::handleConnect_, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
        state_ = CONNECTING;
    }
    else
    {
        state_ = OFFLINE;
        DLOG(INFO) << "Error: " << err.message();
    }
}

void tcp::Controller::handleConnect_(const boost::system::error_code& err,boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        state_ = CONNECTED;
        connection_->start();
        if (!connectedHandler_.empty()) connectedHandler_();
    }
    else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
    {
        /* The connection failed. Try the next endpoint in the list. */
        connection_->socket().close();
        boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
        connection_->socket().async_connect(endpoint,boost::bind(&tcp::Controller::handleConnect_, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        state_ = OFFLINE;
        DLOG(INFO)<< "Error: " << err.message();
    }
}

void tcp::Controller::startAccept_()
{
    auto connection = tcp::Connection::create(io_service_);
    acceptor_.async_accept(connection->socket(),boost::bind(&tcp::Controller::handleAccept_, this, connection, boost::asio::placeholders::error));
}

void tcp::Controller::handleAccept_(tcp::Connection::pointer connection, const boost::system::error_code& error)
{
    if (!error)
    {
        DLOG(INFO) << "Accept new connection";
        connection->start();
        startAccept_();
    }
}


void tcp::Controller::start()
{
    DLOG(INFO)<< "Start tcp::Server";
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 5678);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
    startAccept_();
}

void tcp::Controller::stop()
{
    DLOG(INFO)<< "Stop tcp::Server";
}

