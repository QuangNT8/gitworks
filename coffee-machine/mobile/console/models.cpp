#include "models.h"
#include "manager.h"

#define DATA_ROLE (Qt::UserRole+10)

console::ModelList::~ModelList()
{

}

QList<core::Model*>& console::ModelList::data()
{
    return list_;
}

int console::ModelList::count()
{
    return list_.count();
}

void console::ModelList::add(core::Model *m)
{
    QMutexLocker locker(&mutex_);
    map_[m->id()] = m;
#ifdef Q_OS_IOS
    m->setParent(this);
    m->moveToThread(this->thread());
#else
    m->moveToThread(this->thread());
    m->setParent(this);
#endif
    QMetaObject::invokeMethod(this, "_add", Qt::QueuedConnection,Q_ARG(core::Model*, m));
}

void console::ModelList::update(core::Model *m)
{
    QMutexLocker locker(&mutex_);
    auto iter = map_.find(m->id());
    if (iter != map_.end())
    {
        auto index = list_.indexOf(iter->second);
        if (index != -1)
        {
            iter->second = m;
            QMetaObject::invokeMethod(this, "_update", Qt::QueuedConnection,Q_ARG(int, index), Q_ARG(core::Model*, m));
        }
    }
}

void console::ModelList::remove(uint64_t id)
{
    QMutexLocker locker(&mutex_);
    auto iter = map_.find(id);
    if (iter != map_.end())
    {
        auto c = iter->second;
        map_.erase(iter);
        QMetaObject::invokeMethod(this, "_remove", Qt::QueuedConnection,Q_ARG(core::Model*, c));
    }
}

void console::ModelList::reload(core::Model *m)
{
    int index = list_.indexOf(m);
    if (index != -1)
    {
        emit dataChanged(this->index(index), this->index(index));
    }
}

void console::ModelList::clear()
{
    QMutexLocker locker(&mutex_);
    map_.clear();
    QMetaObject::invokeMethod(this, "_clear", Qt::QueuedConnection);
}

core::Model* console::ModelList::get(uint64_t id)
{
    QMutexLocker locker(&mutex_);
    auto iter = map_.find(id);
    if (iter != map_.end()) return iter->second;
    return nullptr;
}

void console::ModelList::_add(core::Model *m)
{
    QMutexLocker locker(&mutex_);
    beginInsertRows(QModelIndex(), list_.count(), list_.count());
    list_.insert(list_.count(), m);
    endInsertRows();
}

void console::ModelList::_update(int index, core::Model *m)
{
    QMutexLocker locker(&mutex_);
    beginRemoveRows(QModelIndex(), index, index);
    list_.removeAt(index);
    endRemoveRows();

    beginInsertRows(QModelIndex(), index, index);
    list_.insert(index, m);
    endInsertRows();
    qDebug() << "Update done";
}

void console::ModelList::_remove(core::Model *m)
{
    QMutexLocker locker(&mutex_);
    auto index = list_.indexOf(m);
    if (index != -1)
    {
        beginRemoveRows(QModelIndex(), index, index);
        list_.removeAt(index);
        endRemoveRows();
    }
}

void console::ModelList::_clear()
{
    beginResetModel();
    list_.clear();
    endResetModel();
}

QHash<int, QByteArray> console::ModelList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DATA_ROLE] = "model";
    return roles;
}

int console::ModelList::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return list_.size();
}

QVariant console::ModelList::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == DATA_ROLE)
    {
        core::Model* m = list_.at(index.row());
        if (m != nullptr) return QVariant::fromValue<core::Model*>(m);
    }
    return QVariant();
}
