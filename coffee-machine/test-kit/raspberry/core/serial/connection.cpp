#include "connection.h"
#include <glog/logging.h>
#include "controller.h"

serial::Connection::Connection(std::string port):
    port_(core::Engine::instance()->getService()),
    strand_(core::Engine::instance()->getService()),
    portName_(port)
{

}

void serial::Connection::start()
{
    port_.open(portName_);
    port_.set_option(boost::asio::serial_port_base::baud_rate(115200));

    boost::asio::async_read(port_, boost::asio::buffer(&indicator_, 1),
        boost::bind(&serial::Connection::handleReadIndicator_, shared_from_this(),boost::asio::placeholders::error));
}

void serial::Connection::stop()
{
    port_.close();
}

void serial::Connection::handleReadIndicator_(const boost::system::error_code& e)
{
    if (!e)
    {
        if (indicator_ == HEADER_INDICATOR)
        {
            indicator_ = 0;
            boost::asio::async_read(port_, boost::asio::buffer(header_, 3),
                boost::bind(&serial::Connection::handleReadHeader_, shared_from_this(),boost::asio::placeholders::error));
        }
        else
        {
            boost::asio::async_read(port_, boost::asio::buffer(&indicator_, 1),
                boost::bind(&serial::Connection::handleReadIndicator_, shared_from_this(),boost::asio::placeholders::error));
        }
    }
    else
    {
        /*TODO: error*/
    }
}

void serial::Connection::handleReadHeader_(const boost::system::error_code& e)
{
    if (!e)
    {
        length_ = header_[0];
        type_ = header_[1];
        type_ <<= 8;
        type_ += header_[2];

        if (length_ < MAX_PACKET_LENGTH)
        {
            boost::asio::async_read(port_, boost::asio::buffer(data_, length_+1),
                boost::bind(&serial::Connection::handleReadData_, shared_from_this(),boost::asio::placeholders::error));
        }
        else
        {
            boost::asio::async_read(port_, boost::asio::buffer(&indicator_, 1),
                boost::bind(&serial::Connection::handleReadIndicator_, shared_from_this(),boost::asio::placeholders::error));
        }
    }
    else
    {
        /*TODO: error*/
    }
}

void serial::Connection::handleReadData_(const boost::system::error_code& e)
{
    if (!e)
    {
        if (data_[length_] == FOOTER_INDICATOR)
        {
            auto controller = serial::Controller::instance();
            controller->dataReceived(length_, type_, data_);
        }
        boost::asio::async_read(port_, boost::asio::buffer(&indicator_, 1),
            boost::bind(&serial::Connection::handleReadIndicator_, shared_from_this(),boost::asio::placeholders::error));
    }
    else
    {
        /*TODO: error*/
    }
}

void serial::Connection::sendMessage(uint16_t type, uint8_t length, const uint8_t* data)
{
    auto m = boost::make_shared<serial::WriteMessage>(type,length, data);
    strand_.post(boost::bind(&serial::Connection::write_, shared_from_this(), m));
}

void serial::Connection::write_(WriteMessage::pointer m)
{
    if (messages_.empty())
    {
        messages_.push_back(m);
        boost::asio::async_write(port_, boost::asio::buffer(m->data(), m->size()),
                                     strand_.wrap(boost::bind(&serial::Connection::handleWrite_, shared_from_this(), boost::asio::placeholders::error)));
    }
    else messages_.push_back(m);
}


void serial::Connection::handleWrite_(const boost::system::error_code& e)
{
    if (!e)
    {
        messages_.pop_front();
        if (!messages_.empty())
        {
            auto m = messages_.front();
            boost::asio::async_write(port_, boost::asio::buffer(m->data(), m->size()),
                                         strand_.wrap(boost::bind(&serial::Connection::handleWrite_, shared_from_this(), boost::asio::placeholders::error)));
        }
    }
}
