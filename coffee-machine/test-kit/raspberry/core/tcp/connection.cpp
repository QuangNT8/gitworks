#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <glog/logging.h>
#include "connection.h"
#include "engine.h"
#include "operation.h"
#include "utils.h"
#include "controller.h"

tcp::Connection::Connection(boost::asio::io_service& service): strand_(service), socket_(service)
{
}

tcp::Connection::pointer tcp::Connection::create(boost::asio::io_service& service)
{
    return pointer(new Connection(service));
}

boost::asio::ip::tcp::socket& tcp::Connection::socket()
{
    return socket_;
}

void tcp::Connection::start()
{
    lastTime_ = utils::currentTime();
    boost::asio::async_read(socket_, boost::asio::buffer(headerData_, 8),
                            boost::bind(&tcp::Connection::handleReadHeader_, shared_from_this(),boost::asio::placeholders::error));
}

void tcp::Connection::handleReadHeader_(const boost::system::error_code& e)
{
    if (!e)
    {
        readMessage_.reset(new ReadMessage(headerData_));
        if (readMessage_->valid())
        {
            if (readMessage_->protocol()==ReadMessage::EVENT)
            {
                boost::asio::async_read(socket_, boost::asio::buffer(readMessage_->data(), readMessage_->size()),
                                        boost::bind(&tcp::Connection::handleReadData_, shared_from_this(),boost::asio::placeholders::error));

            }
            else
            {
                boost::asio::async_read(socket_, boost::asio::buffer(headerData_, 8),
                                        boost::bind(&tcp::Connection::handleReadId_, shared_from_this(),boost::asio::placeholders::error));
            }
        }
        else
        {
            //socket_.close();
        }
    }
    else
    {
        /*TODO: close connection*/
    }
}

void tcp::Connection::handleReadId_(const boost::system::error_code &e)
{
    if (!e)
    {
        uint8_t* header = (uint8_t*)headerData_;

        uint64_t id = ((uint64_t)header[0] << 56) |
                    (uint64_t)(header[1] << 48) |
                    ((uint64_t)header[2] << 40) |
                    ((uint64_t)header[3] << 32) |
                    ((uint64_t)header[4] << 24) |
                    ((uint64_t)header[5] << 16) |
                    ((uint64_t)header[6] << 8) |
                    ((uint64_t)header[7]);

        readMessage_->setId(id);
        boost::asio::async_read(socket_, boost::asio::buffer(readMessage_->data(), readMessage_->size()),
                                boost::bind(&tcp::Connection::handleReadData_, shared_from_this(),boost::asio::placeholders::error));
    }
    else
    {
        /*TODO: close connection*/
    }
}

void tcp::Connection::handleReadData_(const boost::system::error_code& e)
{
    if (!e)
    {
        static auto engine = core::Engine::instance();
        if (readMessage_->protocol() == ReadMessage::REQUEST)
        {
            auto op = engine->createOperation<tcp::BaseOperation>(readMessage_->type());
            if (op!=nullptr)
            {
                auto t = utils::currentTime();
                this->lastTime_ = t;
                op->setContext(t, shared_from_this(),readMessage_);
                op->run();
            }
            else DLOG(INFO)<<"No tcp::Operation for type:"<<readMessage_->type();
        }
        else if (readMessage_->protocol() == ReadMessage::REPLY)
        {
            auto controller = tcp::Controller::instance();
            auto op = controller->takeOperation(readMessage_->id());
            if (op!=nullptr)
            {
                auto t = utils::currentTime();
                op->setContext(t,shared_from_this(), readMessage_);
                op->run();
            }
            else DLOG(INFO)<<"No request for id:"<<readMessage_->id();
        }
        else  if (readMessage_->protocol() == ReadMessage::EVENT)
        {
            auto op = engine->createOperation<tcp::BaseOperation>(readMessage_->type());
            if (op!=nullptr)
            {
                auto t = utils::currentTime();
                this->lastTime_ = t;
                op->setContext(t, shared_from_this(),readMessage_);
                op->run();
            }
            else DLOG(INFO)<<"No tcp::Operation for type:"<<readMessage_->type();
        }
        this->start();
    }
}

void tcp::Connection::sendMessage(uint64_t id, uint16_t type, const google::protobuf::Message& m)
{
    auto msg = boost::make_shared<WriteMessage>(id,type,m);
    if (msg->valid()) strand_.post(boost::bind(&tcp::Connection::write_, shared_from_this(), msg));
}

void tcp::Connection::write_(WriteMessage::pointer m)
{
    if (messages_.empty())
    {
        messages_.push_back(m);
        boost::asio::async_write(socket_, boost::asio::buffer(m->data(), m->size()),
                                     strand_.wrap(boost::bind(&tcp::Connection::handleWrite_, shared_from_this(), boost::asio::placeholders::error)));
    }
    else messages_.push_back(m);
}

void tcp::Connection::handleWrite_(const boost::system::error_code& e)
{
    if (!e)
    {
        messages_.pop_front();
        if (!messages_.empty())
        {
            auto m = messages_.front();
            boost::asio::async_write(socket_, boost::asio::buffer(m->data(), m->size()),
                                         strand_.wrap(boost::bind(&tcp::Connection::handleWrite_, shared_from_this(), boost::asio::placeholders::error)));
        }
    }
    else DLOG(INFO)<<"Write error";
}
