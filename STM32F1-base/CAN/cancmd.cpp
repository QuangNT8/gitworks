#include "../blink/command.h"
#include "../uart/controller.h"
#include "can.h"

#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]

CAN_COMMAND(CMD_1)
{
	int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("CMD_1 = %d",val);
	/*TODO: implement*/
}

CAN_COMMAND(CMD_2)
{
	int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("CMD_2 = %d",val);
	/*TODO: implement*/
}

CAN_COMMAND(CMD_3)
{
	int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("CMD_3 = %d",val);
	/*TODO: implement*/
}

CAN_COMMAND(CMD_4)
{
    int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("CMD_4 = %d",val);
	/*TODO: implement*/
}

CAN_COMMAND(CMD_5)
{
    int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("CMD_5 = %d",val);
	/*TODO: implement*/
}

CAN_COMMAND(CMD_6)
{
    int32_t val = DECODE_VAL;
    uart::CONTROLLER.printfMessage("CMD_6 = %d",val);
    /*TODO: implement*/
}

CAN_COMMAND_BEGIN
    COMMAND_REG(1, CMD_1)
    COMMAND_REG(2, CMD_2)
    COMMAND_REG(3, CMD_3)
    COMMAND_REG(4, CMD_4)
    COMMAND_REG(5, CMD_5)
    COMMAND_REG(6, CMD_6)
CAN_COMMAND_END
