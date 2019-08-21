#include "models.h"
#include "manager.h"

#define DATA_ROLE (Qt::UserRole+10)

kit::ModelList::~ModelList()
{

}

QList<core::Model*>& kit::ModelList::data()
{
    return list_;
}

int kit::ModelList::count()
{
    return list_.count();
}

void kit::ModelList::add(core::Model *m)
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

void kit::ModelList::update(core::Model *m)
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

void kit::ModelList::remove(std::string id)
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

void kit::ModelList::reload(core::Model *m)
{
    int index = list_.indexOf(m);
    if (index != -1)
    {
        emit dataChanged(this->index(index), this->index(index));
    }
}

void kit::ModelList::clear()
{
    QMutexLocker locker(&mutex_);
    map_.clear();
    QMetaObject::invokeMethod(this, "_clear", Qt::QueuedConnection);
}

core::Model* kit::ModelList::get(std::string id)
{
    QMutexLocker locker(&mutex_);
    auto iter = map_.find(id);
    if (iter != map_.end()) return iter->second;
    return nullptr;
}

void kit::ModelList::_add(core::Model *m)
{
    QMutexLocker locker(&mutex_);
    beginInsertRows(QModelIndex(), list_.count(), list_.count());
    list_.insert(list_.count(), m);
    endInsertRows();
}

void kit::ModelList::_update(int index, core::Model *m)
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

void kit::ModelList::_remove(core::Model *m)
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

void kit::ModelList::_clear()
{
    beginResetModel();
    list_.clear();
    endResetModel();
}

QHash<int, QByteArray> kit::ModelList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DATA_ROLE] = "model";
    return roles;
}

int kit::ModelList::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return list_.size();
}

QVariant kit::ModelList::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == DATA_ROLE)
    {
        core::Model* m = list_.at(index.row());
        if (m != nullptr) return QVariant::fromValue<core::Model*>(m);
    }
    return QVariant();
}
