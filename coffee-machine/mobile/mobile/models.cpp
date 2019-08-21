#include "models.h"
#include "manager.h"

#define DATA_ROLE (Qt::UserRole+10)

home::ModelList::~ModelList()
{

}

QList<core::Model*>& home::ModelList::data()
{
    return list_;
}

int home::ModelList::count()
{
    return list_.count();
}

void home::ModelList::add(core::Model *m)
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

void home::ModelList::clear()
{
    QMutexLocker locker(&mutex_);
    map_.clear();
    QMetaObject::invokeMethod(this, "_clear", Qt::QueuedConnection);
}

core::Model* home::ModelList::get(uint64_t id)
{
    QMutexLocker locker(&mutex_);
    auto iter = map_.find(id);
    if (iter != map_.end()) return iter->second;
    return nullptr;
}

void home::ModelList::_add(core::Model *m)
{
    QMutexLocker locker(&mutex_);
    beginInsertRows(QModelIndex(), list_.count(), list_.count());
    list_.insert(list_.count(), m);
    endInsertRows();
}

void home::ModelList::_clear()
{
    beginResetModel();
    list_.clear();
    endResetModel();
}

QHash<int, QByteArray> home::ModelList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DATA_ROLE] = "model";
    return roles;
}

int home::ModelList::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return list_.size();
}

QVariant home::ModelList::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == DATA_ROLE)
    {
        core::Model* m = list_.at(index.row());
        if (m != nullptr) return QVariant::fromValue<core::Model*>(m);
    }
    return QVariant();
}
