#include "model.h"
#include <QDebug>
using namespace std;

#define DATA_ROLE (Qt::UserRole+10)
core::BaseList::BaseList(QObject *parent): QAbstractListModel(parent)
{
}

core::BaseList::~BaseList()
{
}

QHash<int, QByteArray> core::BaseList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DATA_ROLE] = "model";
    return roles;
}

int core::BaseList::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant core::BaseList::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == DATA_ROLE)
    {
        Model* m = m_list.at(index.row());
        return QVariant::fromValue<Model*>(m);
    }
    return QVariant();
}

const QList<core::Model*>& core::BaseList::data()
{
    return m_list;
}

int core::BaseList::size()
{
    return m_list.size();
}

core::Model* core::BaseList::get(uint16_t id)
{
    QMutexLocker locker(&mutex);
    return m_map.value(id, nullptr);
}

void core::BaseList::reload(Model* m)
{
    QMetaObject::invokeMethod(this, "_reload", Qt::QueuedConnection,Q_ARG(Model*, m));
}

void core::BaseList::add(Model* m)
{
    QMutexLocker locker(&mutex);
    m->moveToThread(this->thread());
    m_map.insert(m->id, m);
    QMetaObject::invokeMethod(this, "_add", Qt::QueuedConnection,Q_ARG(Model*, m));
    //qDebug()<<m_list.size();
}

void core::BaseList::addMulti(QList<Model*> list)
{
    QMutexLocker locker(&mutex);
    foreach (Model* m, list)
    {
        m->moveToThread(this->thread());
        m_map.insert(m->id, m);
    }
    QMetaObject::invokeMethod(this, "_addMulti", Qt::QueuedConnection,Q_ARG(QList<Model*>, list));
}

void core::BaseList::remove(uint16_t id)
{
    QMutexLocker locker(&mutex);
    auto iter = m_map.find(id);
    if(iter != m_map.end())
    {
        auto m = iter.value();
        m_map.erase(iter);
        QMetaObject::invokeMethod(this, "_remove", Qt::QueuedConnection,Q_ARG(Model*, m));
    }
}


void core::BaseList::_add(Model* m)
{
    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(m);
    endInsertRows();
}

void core::BaseList::_addMulti(QList<Model*> list)
{
    beginResetModel();
    foreach (Model* m, list)
    {
        m_list.append(m);
    }
    endResetModel();
}

void core::BaseList::_remove(Model* m)
{
    int index = m_list.indexOf(m);
    if(index != -1)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_list.removeAt(index);
        endRemoveRows();
    }
}

void core::BaseList::_reload(Model* m)
{
    int index = m_list.indexOf(m);
    if(index != -1) emit dataChanged(this->index(index), this->index(index));
}
