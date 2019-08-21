#ifndef UARTREADER_H
#define UARTREADER_H
#include "core/operation.h"
#include "controller.h"
#include <QMutex>
#include <QMutexLocker>

namespace serial
{

class Reader: public core::Operation
{
    Q_OBJECT
public:
    Reader(): core::Operation()
    {
        reading_ = false;
    }
    void run()
    {
        static auto uart = serial::Controller::instance();
        uart->readData();
        QMutexLocker locker(&mutex);
        reading_ = false;
    }

    void beginRead()
    {
        QMutexLocker locker(&mutex);
        if (!reading_)
        {
            reading_ = true;
            this->fire();
        }
    }
private:
    bool reading_;
    QMutex mutex;
};

}
#endif // UARTREADER_H
