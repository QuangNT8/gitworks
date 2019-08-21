#include "manager.h"
#include "core/views.h"
#include "operation/operations.h"
#include "../raspberry/ann/neuronnetwork.h"
#include <QFile>
void console::Manager::init()
{
    core::Views::instance()->registerComponent("Console", this);
    connect(tcp::Controller::instance(),&tcp::Controller::stateChanged,this, &Manager::tcpStateChanged);
    REGISTRATION(operations)
    core::Views::instance()->setWidth(1366);
    core::Views::instance()->setHeight(768);
    tanks = new console::ModelList(this);
    for (uint32_t index = 0; index < TANK_COUNT; index++)
    {
        data::bj::Tank t;
        t.set_index(index);
        t.set_volume(0);
        t.set_liquidid(0);
        t.set_pouring(false);
        tanks->add(new console::Tank(t));
    }
    data::bj::Motor left;
    left.set_position(0);
    leftMotor = new console::Motor(left);

    data::bj::Motor right;
    right.set_position(100);
    rightMotor = new console::Motor(right);

    recipes = new console::ModelList(this);
    liquids = new console::ModelList(this);
    orderid = 0;
    debugMode = false;
    machineReady = false;
}

void console::Manager::start()
{
    tcp::Controller::instance()->open(SERVER_IP, SERVER_PORT);
}

void console::Manager::tcpStateChanged()
{
    auto state = tcp::Controller::instance()->state;
    switch (state)
    {
        case tcp::Controller::CONNECTED:
        {
            auto op = new machine::Login();
            op->fire();
            break;
        }
        case tcp::Controller::DISCONNECTED:
        {
            break;
        }
    }
}

quint32 console::Manager::getIngredient(core::Model* liquid, core::Model* recipe, DataIngredientFormat type)
{
    auto r = Manager::instance()->recipes->get(recipe->id());
    if(r != nullptr)
    {
        auto rp = qobject_cast<console::Recipe*>(r);
        for(auto& l: rp->data().ingredients())
            if(l.liquidid() == liquid->id())
            {
                switch (type)
                {
                    case VOLUME: return l.volume();
                    case MIN: return l.min();
                    case MAX: return l.max();
                    default: return 0;
                }
            }
    }
    return 0;
}
void console::Manager::sendLog(QString tag, QString log)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    emit this->messageReceived(time, tag, log);
}

QString console::Manager::getTime()
{
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

QStringList console::Manager::getLiquids()
{
    QStringList list;
    list.append("-- EMPTY --");
    for (auto& l: this->liquids->data())
    {
        list.append(qobject_cast<console::Liquid*>(l)->get_name());
    }
    return list;
}

quint64 console::Manager::getLiquidByName(QString name)
{
    for (auto& l: this->liquids->data())
    {
        auto liquid = qobject_cast<console::Liquid*>(l);
       if (name == liquid->get_name()) return liquid->data().id();
    }
    return 0;
}

QString console::Manager::getLiquiNamedById(quint64 id)
{
    if (id == 0) return "None";
    auto liquid = this->liquids->get(id);
    if (liquid != nullptr)
    {
        return qobject_cast<console::Liquid*>(liquid)->get_name();
    }
    return QString();
}
