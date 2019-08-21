#ifndef KIT_PUMP_H
#define KIT_PUMP_H
#include "engine.h"
#include "serial/connection.h"
#include "serial.h"
#include "config.h"

namespace kit
{

class Pump
{
public:
    typedef boost::shared_ptr<Pump> pointer;

    enum State {OPEN = 0, CLOSED, CLOSING};
    Pump(serial::Connection::pointer con):connection_(con) { state_ = CLOSED; }

    void turnOn()
    {
        //Check connection
        if(connection_ == nullptr) return;

        connection_->sendMessage(kit::TurnOnPump, 0, 0);
        state_ = kit::Pump::OPEN;
    }

    void turnOff()
    {
        //Check connection
        if(connection_ == nullptr) return;

        state_ = kit::Pump::CLOSING;
        connection_->sendMessage(kit::TurnOffPump, 0, 0);
    }

    State state() { return state_; }
    void update(State state)
    {
        if (state != state_)
        {
            state_ = state;
        }
    }

private:
    serial::Connection::pointer connection_;
    State state_ = CLOSED;
};

}

#endif // KIT_PUMP_H
