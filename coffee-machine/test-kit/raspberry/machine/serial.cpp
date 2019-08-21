#include "manager.h"
#include "serial.h"
#include "comm.kit.pb.h"

void kit::Manager::serialReceived(uint16_t length, uint16_t type, uint8_t* data)
{
    comm::kit::MessageReceived event;

    switch (type)
    {
        case MessageReceived:
        {
            DLOG(INFO) << "MessageReceived "<<(char *)data;
            event.set_text((char *)data);
            Manager::instance()->pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
            break;
        }
        case ValveOpened:
        {
            this->valveOpened();
            break;
        }
        case ValveClosed:
        {
            this->valveClosed();
            break;
        }
        case WeightChanged:
        {
            //Simple "ST,NT,+   1.23 g"
//            DLOG(INFO) << "WeightChanged data = "<<(char *)data;
            std::string s((char *)data);
            bool stable = (s.substr(0,2) == "ST");
//            DLOG(INFO) <<"stable "<<stable;
            s.erase(0,6); //Remove "ST,NT,"
            bool positive = (s.substr(0,1) == "+");
//            DLOG(INFO) <<"positive = "<<positive;
            s.erase(0, 1); //remove "+"
            s.erase(s.length() - 1, 1); //Remove "g"
//            DLOG(INFO) <<"w = "<<s;

            double w = atof(s.c_str());
            if (!positive) w = -w;
            this->weightChanged(w,stable);
            break;
        }
        case PumpOpened:
        {
            DLOG(INFO) << "PumpOpened";
            this->pumpOpened();
            break;
        }
        case PumpClosed:
        {
            DLOG(INFO) << "PumpClosed";
            this->pumpClosed();
            break;
        }
        case MachineReady:
        {
            DLOG(INFO) << "MachineReady";
            event.set_text("MachineReady");
            Manager::instance()->pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
            break;
        }
        case MachineError:
        {
            DLOG(INFO) << "MachineError";
            event.set_text("MachineError");
            Manager::instance()->pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
            break;
        }

        default:

            DLOG(INFO) << "SerialReceived";
            event.set_text("SerialReceived");
            Manager::instance()->pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
            break;

      }
}
