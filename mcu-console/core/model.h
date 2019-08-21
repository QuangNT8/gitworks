#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include <QMutex>
#include <string>
namespace core
{

class Model: public QObject
{
    Q_OBJECT
public:
    uint16_t id;
    Model(QObject *parent = 0): QObject(parent){}
    virtual ~Model(){}
};

class BaseList: public QAbstractListModel
{
    Q_OBJECT
public:
    BaseList(QObject *parent = 0);
    virtual ~BaseList();

    const QList<Model*>& data();
    int size();
    Model* get(uint16_t id);
    void reload(Model* m);
    void add(Model* m);
    void addMulti(QList<Model*> list);
    void remove(uint16_t id);
protected:
    /* Override */
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // variables
    QList<Model*> m_list;
    QHash<uint16_t, Model*> m_map;
    QMutex mutex;
protected slots:
    void _add(Model* m);
    void _addMulti(QList<Model*> list);
    void _remove(Model* m);
    void _reload(Model* m);
};

}

namespace std
{
    inline uint qHash(const std::string &key, uint seed)
    {
        Q_UNUSED(seed)
        return std::hash<std::string>()(key);
    }
}

#endif // MODEL_H

