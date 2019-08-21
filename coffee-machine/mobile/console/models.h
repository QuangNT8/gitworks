#ifndef MODELS_H
#define MODELS_H

#include "data.bj.pb.h"
#include "model.h"
#include <unordered_map>
#include <QAbstractListModel>
#include <QMutex>
#include <QDebug>

namespace console
{

class ModelList: public QAbstractListModel
{
    Q_OBJECT
public:
    ModelList(QObject *parent = 0) : QAbstractListModel(parent) {}
    virtual ~ModelList();

    void add(core::Model* m);
    void remove(uint64_t id);
    void update(core::Model* m);
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
    void _update(int index, core::Model *m);
    void _remove(core::Model *m);
    void _clear();
};

MODEL(data::bj::Motor, Motor)
Q_OBJECT
    UINT32_(position)
END

ENTITY(data::bj::Liquid, Liquid)
Q_OBJECT
    STRING_(name)
END

MODEL(data::bj::Tank, Tank)
Q_OBJECT
    UINT32_(index)
    DOUBLE_(volume)
    UINT64_(liquidid)
    BOOL_(pouring)
END

ENTITY(data::bj::Recipe, Recipe)
Q_OBJECT
    STRING_(name)
    STRING_(description)
    STRING_(image)

END

}

#endif // MODELS_H
