#ifndef TYPE_H
#define TYPE_H

namespace console
{
    enum CommandType
    {
        PRINT = 1,
        PLOT,
        OSC,
        UPDATE_FIELD
    };

    enum PlotDataType
    {
        UINT8=0,
        INT8,
        UINT16,
        INT16,
        UINT32,
        INT32
    };
}

#endif // TYPE_H
