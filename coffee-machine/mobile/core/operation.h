#ifndef CORE_OPERATION_H
#define CORE_OPERATION_H

#include <QRunnable>
#include <QObject>
#include <QThreadPool>
#include <QQmlEngine>
#include <QDebug>
#include "engine.h"
#include <thread>


#define OPERATION_REG(module, name)\
{\
    QString tmp = #module;\
    tmp += ".";\
    tmp += #name;\
    qmlRegisterType<module::name>(#module, 1, 0, #name);\
    core::Engine::instance()->registerOperation(tmp, new core::ObjectCreator<module::name >);\
}

#define COMPONENT_REG(module, name)\
{\
    engine->registerComponent(new module::name());\
}

#define OPERATION(module, name)\
namespace module\
{\
    class name: public core::Operation\
    {\
    public:\
    void run(){this->execute();finish(0);}\
        void execute();

#define OP_EXECUTE(module, name) void module::name::execute()

namespace core
{

class Operation: public QObject, public QRunnable
{
    Q_OBJECT
private:
    Q_SIGNAL void done();
public:
    Q_SIGNAL void finished(quint32 code);
    Operation()
    {
        auto engine = Engine::instance();
#ifdef Q_OS_IOS
        setParent(engine);
        moveToThread(engine->thread());
#else
        moveToThread(engine->thread());
        setParent(engine);
#endif
        setAutoDelete(false);
        //connect(this, &Operation::finished,this, [=]{this->deleteLater();}, Qt::QueuedConnection);
        connect(this, SIGNAL( done() ),this, SLOT( killMe()) , Qt::QueuedConnection);
    }
    virtual ~Operation(){}

    virtual void run()
    {
        qDebug()<<"NullOperation";
        finish(0);
    }

    Q_INVOKABLE void fire()
    {
        QThreadPool::globalInstance()->start(this);
    }
    void finish(quint32 code)
    {
        //DEBUG
        //qDebug()<<"emit finished " << code;
        emit finished(code);
        emit done();
    }
private slots:
    void killMe()
    {
        //DEBUG
        //qDebug()<<"kill me ";
        this->deleteLater();
    }
};

}

#endif // CORE_OPERATION_H
