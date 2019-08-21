#ifndef KIT_TANK_H
#define KIT_TANK_H
#include "engine.h"
#include "serial/connection.h"
#include "serial.h"
#include "config.h"
#include <random>

namespace kit
{

class Tank
{
public:
    typedef boost::shared_ptr<Tank> pointer;
    Tank(serial::Connection::pointer con):connection_(con) { size_ =  1900.0; }

    void open(uint16_t time)
    {
        if(connection_ == nullptr) return;

        uint8_t data[2];
        data[0] = ((time>>8) & 0xFF);
        data[1] = (time & 0xFF);

        connection_->sendMessage(kit::OpenValve, 2, data);
    }

    void updateSize(double size) { size_ = size; }
    double size(){return size_;}
private:
    double size_;
    serial::Connection::pointer connection_;
};

}

#endif // KIT_TANK_H
