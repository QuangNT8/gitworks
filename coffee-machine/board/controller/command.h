#ifndef BJ_COMMAND_H
#define BJ_COMMAND_H
#include "motor/define.h"

#if MOTOR
#define  ROBOT   bj::LEFT
#else
#define  ROBOT   bj::RIGHT
#endif

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

#endif // COMMAND_H
