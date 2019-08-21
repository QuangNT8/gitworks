#ifndef RECENTLIST_H
#define RECENTLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include <QMutex>
#include "core/model.h"
#include "core/define.h"

MODEL(RecentItem)
Q_OBJECT
    READONLY(QString, name)
    READONLY(QString, file)
END

namespace console
{

class RecentList: public QAbstractListModel
{
    Q_OBJECT
public:
    RecentList(QObject *parent = 0);
    virtual ~RecentList();
    void add(QString name, QString file);
    void save(QString fileName);
    void load(QString fileName);
protected:
    /* Override */
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // variables
    QList<RecentItem*> m_list;
};

}

#endif // RECENTLIST_H
