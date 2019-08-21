#ifndef MODELS_H
#define MODELS_H

#include "model.h"
#include "data.bj.pb.h"
#include <unordered_map>
#include <QAbstractListModel>
#include <QMutex>
#include <QDebug>

namespace home
{
class ModelList: public QAbstractListModel
{
    Q_OBJECT
public:
    ModelList(QObject *parent = 0) : QAbstractListModel(parent) {}
    virtual ~ModelList();

    void add(core::Model* m);
    core::Model* get(uint64_t id);
    QList<core::Model*>& data();
    void reload(core::Model* m);
    void clear();
    int count();

private:
    QList<core::Model*> list_;
    std::unordered_map<uint64_t, core::Model*> map_;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QMutex mutex_;
private slots:
    void _add(core::Model *m);
    void _clear();
};

ENTITY(data::bj::Liquid, Liquid)
Q_OBJECT
    STRING_(name)
END


ENTITY(data::bj::Recipe, Recipe)
Q_OBJECT
    STRING_(name)
    STRING_(description)
    STRING_(image)
END

ENTITY(data::bj::Session, Session)
Q_OBJECT
    STRING_(host)
    UINT32_(port)
END
}

#endif // MODELS_H
