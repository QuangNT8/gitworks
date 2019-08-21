#ifndef MODELS_H
#define MODELS_H

#include "model.h"
#include <unordered_map>
#include <QAbstractListModel>
#include <QMutex>
#include <QDebug>
#include "data.kit.pb.h"
#include "plotline.h"
#include <QLineSeries>
namespace kit
{

class ModelList: public QAbstractListModel
{
    Q_OBJECT
public:
    ModelList(QObject *parent = 0) : QAbstractListModel(parent) {}
    virtual ~ModelList();

    void add(core::Model* m);
    void remove(std::string id);
    void update(core::Model* m);
    core::Model* get(std::string id);
    QList<core::Model*>& data();
    void reload(core::Model* m);
    void clear();
    int count();

private:
    QList<core::Model*> list_;
    std::unordered_map<std::string, core::Model*> map_;
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

ENTITY(data::kit::Project, Project)
Q_OBJECT
    STRING_(id)
    STRING_(name)
    STRING_(file)
    UINT64_(date)
    UINT32_(total)
    UINT32_(remain)
    DOUBLE_(tanksize)

    Q_INVOKABLE void samples(QLineSeries* lineChart)
    {
        QMutexLocker locker(&mutex_);
        for (auto& s: this->data().samples())
        {
            lineChart->append(s.index(), s.vexpected());
        }
    }
END

}

#endif // MODELS_H
