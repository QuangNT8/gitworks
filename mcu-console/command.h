#ifndef COMMANDS_H
#define COMMANDS_H
#include "serial/controller.h"
#include "core/views.h"
#include "console.h"

SERIAL_COMMAND(Print)
{
    QString msg((char*)data);
    core::Views::instance()->showMessage(msg);
}

SERIAL_COMMAND(Plot)
{
    static auto c = console::Controller::instance();
    c->displayPlot(data, length);
}

SERIAL_COMMAND(Oscilloscope)
{
    static auto c = console::Controller::instance();
    c->displayOsc(data, length);
}

SERIAL_COMMAND(UpdateField)
{
    static auto c = console::Controller::instance();
    uint16_t cmd = ((uint16_t)data[0] << 8) | ((uint16_t)data[1]);
    int16_t val = ((int16_t)data[2] << 8) | ((int16_t)data[3]);
    //qDebug()<<"cmd:"<<cmd<<",val:"<<val;
    c->upadateField(cmd, val);
}


#endif // COMMANDS_H
