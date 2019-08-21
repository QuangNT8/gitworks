#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H

#include "engine.h"
#include "connection.h"
#include <glog/logging.h>

COMPONENT(serial, Controller)
public:
    Controller(){}

    void init()
    {
        DLOG(INFO) << "Serial init";
    }
    void start()
    {
        DLOG(INFO) << "Serial start";
        //startConnections();
    }
    void stop()
    {
        for (auto& c: connections_)
        {
            c.second->stop();
        }
    }
    void startConnections()
    {
        for (auto& c: connections_)
        {
            c.second->start();
        }
    }

    serial::Connection::pointer addConnection(std::string port)
    {
        auto c = boost::make_shared<serial::Connection>(port);
        connections_[port] = c;
        return c;
    }

    Connection::pointer getConnection(std::string port)
    {
        auto it = connections_.find(port);
        if (it != connections_.end()) return it->second;
        return nullptr;
    }

    void dataReceived(uint16_t length, uint16_t type, uint8_t* data, Connection::pointer con)
    {
        if (!receivedHandler_.empty()) receivedHandler_(length, type, data, con);
    }

    void setReceivedHandler(boost::function<void(uint16_t, uint16_t, uint8_t*, Connection::pointer)> handler){receivedHandler_ = handler;}

private:
    boost::function<void(uint16_t, uint16_t, uint8_t*, Connection::pointer)> receivedHandler_;
    std::unordered_map<std::string, Connection::pointer> connections_;

COMPONENT_END

#endif // CONTROLLER_H
