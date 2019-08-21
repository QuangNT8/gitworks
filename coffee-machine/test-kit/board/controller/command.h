#ifndef KIT_COMMAND_H
#define KIT_COMMAND_H

namespace Tkit
{

enum Command
{
    MessageReceived = 1,
    OpenValve,
    ValveOpened,
    ValveClosed,
    WeightChanged,
    TurnOnPump,
    TurnOffPump,
    PumpOpened,
    PumpClosed,
    MachineReady,
    MachineError,
};


}

#endif // KIT_COMMAND_H
