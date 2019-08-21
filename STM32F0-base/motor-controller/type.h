#ifndef DEBUGER_TYPE_H
#define DEBUGER_TYPE_H

enum CommandType
{
    CMD_SKP = 1,
    CMD_SKI = 2,
    CMD_SPEED = 3,
    CMD_ACCEL = 4,
    CMD_JERK = 5,
    CMD_DKP = 7,
    CMD_DKI = 8,
    CMD_QKP = 9,
    CMD_QKI = 10,
    CMD_ALIGN_MOTOR = 12,
    CMD_START_MOTOR = 13,
    CMD_STOP_MOTOR = 14,
};

enum PlotIndex
{
    PLOT_CURRENT_SPEED = 0,
    PLOT_REFERENCE_SPEED = 1,
    PLOT_SPEED_ERROR = 2,
    PLOT_ID_ERROR = 3,
    PLOT_IQ_ERROR = 4,
};

#endif
