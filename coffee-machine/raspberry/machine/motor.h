#ifndef MACHINE_MOTOR_H
#define MACHINE_MOTOR_H
#include "engine.h"
#include "serial/connection.h"
#include "serial.h"
#include "utils.h"
#include "manager.h"

namespace bj
{

class Motor
{
public:
    typedef boost::shared_ptr<Motor> pointer;
    Motor(serial::Connection::pointer con): connection_(con){}

    void moveTo(uint32_t position)
    {
        setpoint_ = position;

        uint8_t data[4];
        data[0] = ((position>>24) & 0xFF);
        data[1] = ((position>>16) & 0xFF);
        data[2] = ((position>>8) & 0xFF);
        data[3] = (position & 0xFF);
        connection_->sendMessage(bj::MoveMotor, 4, data);
    }

    uint32_t setpoint(){return setpoint_;}
    uint32_t position(){return position_;}

    void updatePosition(uint32_t position){position_ = position;}
    serial::Connection::pointer getConnection(){return connection_;}

    void reset(){connection_->sendMessage(bj::ResetMachine, 0, 0);setpoint_ = 0;}
    bool hasUpdatePosition()
    {
        uint64_t currentTime = utils::currentTime();
        if (currentTime - positionLastUpdate_ > POSITION_UPDATE_CYCLE)
        {
            positionLastUpdate_ = currentTime;
            return 1;
        }
        return 0;
    }
    void updateConnection(serial::Connection::pointer con){connection_ = con;}
private:
    serial::Connection::pointer connection_;
    uint32_t position_ = 0;
    uint32_t setpoint_ = 0;
    uint64_t positionLastUpdate_ = 0;
};

}

#endif // MACHINE_MOTOR_H
