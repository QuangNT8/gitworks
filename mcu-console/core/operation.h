#ifndef CORE_OPERATION_H
#define CORE_OPERATION_H

#include <QRunnable>
#include <QObject>
#include <QThreadPool>
#include <QQmlEngine>
#include <QDebug>
#include "engine.h"
#include "define.h"

#define OPERATION_REG(module, name)\
{\
    QString tmp = #module;\
    tmp += ".";\
    tmp += #name;\
    qmlRegisterType<module::name>(#module, 1, 0, #name);\
    core::Events::instance()->registerOperation(tmp, new core::OperationCreator<module::name >);\
}

namespace core
{

class Operation: public QObject, public QRunnable
{
    Q_OBJECT
    EVENT(finished())
public:
    Operation(): QObject()
    {
        setAutoDelete(false);
        connect(this, &Operation::finished,[this]{this->deleteLater();});
    }
    virtual ~Operation(){}

    virtual void run()
    {
        qDebug()<<"NullOperation";
        finish();
    }

    Q_INVOKABLE void fire()
    {
        QThreadPool::globalInstance()->start(this);
       // Engine::instance()->schedule(this);
    }
    void finish()
    {
        emit finished();
    }
};

}

#endif // CORE_OPERATION_H
