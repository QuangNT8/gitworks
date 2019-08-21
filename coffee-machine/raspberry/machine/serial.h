#ifndef SERIAL_H
#define SERIAL_H

namespace bj
{

enum Command
{
    MessageReceived = 1,
    MoveMotor,
    MotorStarted,
    MotorStopped,
    OpenValve,
    ValveOpened,
    ValveClosed,
    PositionChanged,
    ResetMachine,
    MachineReady,
    MachineError
};

enum Robot
{
    LEFT = 1,
    RIGHT
};

}

#endif // SERIAL_H
