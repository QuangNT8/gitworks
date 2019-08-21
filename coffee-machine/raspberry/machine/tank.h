#ifndef MACHINE_TANK_H
#define MACHINE_TANK_H
#include "engine.h"
#include "ann/neuronnetwork.h"
#include "data.bj.pb.h"
#include "serial/connection.h"
#include "serial.h"
#include "config.h"

namespace bj
{

class Tank
{
public:
    typedef boost::shared_ptr<Tank> pointer;
    Tank(uint16_t port):port_(port) {}

    void open(uint16_t time)
    {
        //Check connection
        if(connection_ == nullptr) return;

        uint8_t data[4];

        data[0] = ((time>>8) & 0xFF);
        data[1] = (time & 0xFF);

        data[2] = ((port_ >> 8) & 0xFF);
        data[3] = (port_ & 0xFF);

        connection_->sendMessage(bj::OpenValve, 4, data);
    }

    void close(double v)
    {
        volume_ -= v;
    }

    bool reserve(uint64_t liquid, double v)
    {
        if (liquid == liquid_ && available_ > v)
        {
            available_ -= v;
            return true;
        }
        return false;
    }

    bool check(uint64_t liquid, double v)
    {
        if (liquid == liquid_ && available_ > v) return true;
        return false;
    }

    void updateValve(bool closed)
    {
        closed_ = closed;
    }
    void updateVolume(double vol)
    {
        volume_ = vol;

        //for loop test
        volume_ = 2000;
        available_ = 2000;
    }

    void change(uint64_t liquid, uint32_t vol )
    {
        liquid_ = liquid;
        volume_ = vol;
        available_ = vol;
    }
    void updateConnection(serial::Connection::pointer con)
    {
        connection_ = con;
    }
    double volume(){return volume_;}
    uint64_t liquid() {return liquid_;}
    uint16_t port(){return port_;}
    bool closed(){return closed_;}
private:
    double available_;
    double volume_ = 0;
    bool closed_ = true;
    uint64_t liquid_ = 0;
    serial::Connection::pointer connection_;
    uint16_t port_;
};

}

#endif // MACHINE_TANK_H
