#include "command.h"
#include "type.h"

SERIAL_COMMAND_BEGIN
    COMMAND_REG(console::PRINT, Print)
    COMMAND_REG(console::PLOT, Plot)
    COMMAND_REG(console::OSC, Oscilloscope)
    COMMAND_REG(console::UPDATE_FIELD, UpdateField)
SERIAL_COMMAND_END

