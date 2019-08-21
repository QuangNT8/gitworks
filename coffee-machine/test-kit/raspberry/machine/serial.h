#ifndef SERIAL_H
#define SERIAL_H

namespace kit
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

#endif // SERIAL_H
