#include "manager.h"
#include "serial.h"
#include "schedule.h"
#include "comm.machine.pb.h"

void bj::Manager::serialReceived(uint16_t length, uint16_t type, uint8_t* data, serial::Connection::pointer con)
{
    comm::machine::MessageReceived event;

    auto schedule = bj::Schedule::instance();
    auto manager = bj::Manager::instance();

    switch (type)
    {
    case MessageReceived:
    {
        event.set_text(reinterpret_cast<char*>(data));
        manager->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);
        break;
    }
    case MotorStarted: //robot:2bytes(uint16), position: 4 bytes(uint32)
    {        
        if (length != 6) break;
        uint16_t robot = (data[0] <<8) + data[1];
        uint32_t position = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + data[5];
//        schedule->motorStarted(robot, position);
        break;
    }
    case MotorStopped:
    {        
        if (length != 6) break;
        uint16_t robot = (data[0] <<8) + data[1];
        uint32_t position = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + data[5];
        schedule->motorStopped(robot, position);
        break;
    }
    case ValveOpened:
    {
        //test
        event.set_text("ValveOpened");
        manager->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);

        uint16_t index = (data[0] <<8) + data[1];
        for (uint32_t i = 0; i < TANK_COUNT; i++)
        {
            auto tank = Manager::instance()->tanks_[i];
            if (tank != nullptr && tank->port() == index)
            {
                event.set_text("Valve " +std::to_string(index + 1)+" opened");
                manager->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);
                tank->updateValve(false);

                comm::machine::StateChanged state;
                auto add = state.add_tanks();
                add->set_index(i);
                add->set_volume(tank->volume());
                add->set_liquidid(tank->liquid());
                add->set_pouring(tank->closed());

                manager->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);

//                bj::Schedule::instance()->valveOpened(tank->index());
                break;
            }
        }
        break;
    }
    case ValveClosed:
    {
        //test
        event.set_text("ValveClosed");
        manager->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);

        uint16_t index = (data[0] <<8) + data[1];
        for (uint32_t i = 0; i < TANK_COUNT; i++)
        {
            auto tank = Manager::instance()->tanks_[i];
            if (tank != nullptr && tank->port() == index)
            {
                event.set_text("Valve " +std::to_string(index + 1)+" closed");
                manager->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);
                tank->updateValve(true);

                comm::machine::StateChanged state;
                auto add = state.add_tanks();
                add->set_index(i);
                add->set_volume(tank->volume());
                add->set_liquidid(tank->liquid());
                add->set_pouring(tank->closed());

                manager->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);

                if(bj::Schedule::instance() == nullptr)
                    DLOG(INFO) << "schedule not initialized !";
                bj::Schedule::instance()->valveClosed(tank->port());
                break;
            }
        }
        break;
    }
    case PositionChanged:
    {
        if (length != 6) break;
        uint16_t robot = (data[0] <<8) + data[1];
        uint32_t position = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + data[5];

        comm::machine::StateChanged state;        
        if (robot == bj::LEFT)
        {
            if(this->leftMotor_->hasUpdatePosition())
            {
                auto add = state.mutable_leftmotors();
                add->set_position(position);

                this->leftMotor()->updatePosition(position);
            }
        }
        else if (robot == bj::RIGHT)
        {
            if(this->rightMotor_->hasUpdatePosition())
            {
                auto add = state.mutable_rightmotors();
                add->set_position(position);

                this->rightMotor()->updatePosition(position);
            }
        }
        if(state.has_leftmotors() || state.has_rightmotors())
            this->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);
        break;
    }

    case MachineReady: //ready signal from left or right motor
    {
        if (length != 2) break;

        boost::mutex::scoped_lock lock(stateReadyMutex_);

        comm::machine::StateChanged state;
        uint16_t robot = (data[0] <<8) + data[1];
        if (robot == bj::LEFT)
        {
            manager->leftMotor()->updatePosition(0);
            /*TODO: setpoint = 0*/
            state.mutable_leftmotors()->set_position(0);
            if(machineState_ == RIGHT_READY) machineState_ = READY;
            else if(machineState_ == NOT_READY) machineState_ = LEFT_READY;

            //set connection again
            if(machineStart)
            {
                this->updateConnectionTank(con);
                this->leftMotor_->updateConnection(con);
                if(machineState_ == READY) machineStart = false;
            }

        }
        else if (robot == bj::RIGHT)
        {
            this->rightMotor()->updatePosition(0);
            /*TODO: setpoint = 0*/
            state.mutable_rightmotors()->set_position(0);
            if(machineState_ == LEFT_READY) machineState_ = READY;
            else if(machineState_ == NOT_READY) machineState_ = RIGHT_READY;

            //set connection again
            if(machineStart)
            {
                this->rightMotor_->updateConnection(con);
                if(machineState_ == READY) machineStart = false;
            }
        }

        if(state.has_leftmotors() || state.has_rightmotors())
        {
            if(machineState_ == READY)
            {
                this->debugMode_ = this->setMode_;
                /*TODO: send event*/
                state.set_debugmode(this->setMode_);
                state.set_machineready(this->machineReady());
            }
            this->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);

        }
        break;
    }
    case MachineError:
    {
        //test
        event.set_text("MachineError");
        this->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);

//        bj::Schedule::instance()->machineCrashed();
        break;
    }

    default:
        //test
        event.set_text("SerialReceived");
        this->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);

        break;

    }
}
