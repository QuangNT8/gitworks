#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include <QMutex>
#include <string>
#include "core/model.h"
#include "core/define.h"
#include <QColor>
#include <QUrl>
#include <QXYSeries>
#include "plotline.h"

using namespace core;

enum ItemType
{
    ITEM_SECTION = 0,
    ITEM_FIELD,
    ITEM_BUTTON,
    ITEM_PLOT,
    ITEM_PLOT_HEADER
};

MODEL(FieldItem)
Q_OBJECT
    READONLY(quint16, command)
    READONLY(quint16, type)
    PROPERTY(QString, name)
    PROPERTY(qint32, min)
    PROPERTY(qint32, max)
    PROPERTY(qint32, value)
    PROPERTY(bool, readonly)
END

MODEL(ButtonItem)
Q_OBJECT
    READONLY(quint16, command)
    READONLY(quint16, type)
    PROPERTY(QString, name)
END

MODEL(SectionItem)
Q_OBJECT
    READONLY(quint16, type)
    PROPERTY(QString, name)
END

MODEL(PlotItem)
Q_OBJECT
    READONLY(quint16, type)
    PROPERTY(QString, name)
    PROPERTY(QColor, color)
    READONLY(QtCharts::QXYSeries*, plot)
    PROPERTY(quint16, chart)
    FIELD(DataLine*, line)
    PROPERTY(qint16, threshold)
    PROPERTY(bool, highSpeed)
END

MODEL(ChartItem)
Q_OBJECT
    READONLY(quint16, type)
    READONLY(quint16, index)
    PROPERTY(QString, name)
    PROPERTY(qint16, min)
    PROPERTY(qint16, max)
    PROPERTY(bool, visible)
END

namespace console
{

class CommandList: public QAbstractListModel
{
    Q_OBJECT
    PROPERTY(QString, projectName)
    PROPERTY(QString, filePath)
    PROPERTY(QUrl, folder)
public:
    CommandList(QObject *parent = 0);
    virtual ~CommandList();

    const QList<Model*>& data();
    int size();
    void reload(Model* m);
    void add(Model* m);
    void addMulti(QList<Model*> list);
    void copyFrom(CommandList* list);

    QList<PlotItem*>& plots(){return plotList_;}
    ChartItem* chart1(){return chart1_;}
    ChartItem* chart2(){return chart2_;}

    Q_INVOKABLE void save();
    Q_INVOKABLE bool load(QString fileName);
    Q_INVOKABLE void remove(Model* m);
    Q_INVOKABLE void createNewModel();
    Q_INVOKABLE void newField(Model* section, QString name);
    Q_INVOKABLE void newButton(Model* section,QString name);
    Q_INVOKABLE core::Model* newSection(QString name);
    Q_INVOKABLE void newPlot(Model* chart,QString name);

    Q_INVOKABLE void moveUp(Model* m);
    Q_INVOKABLE void moveDown(Model* m);

    Q_INVOKABLE void movePlotUp(Model* m);
    Q_INVOKABLE void movePlotDown(Model* m);


protected:
    /* Override */
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // variables
    QList<Model*> m_list;
    QList<PlotItem*> plotList_;
    ChartItem* chart1_;
    ChartItem* chart2_;
    QMutex mutex;
    uint16_t nextCommand_;
protected slots:
    void _add(Model* m);
    void _addMulti(QList<Model*> list);
    void _remove(Model* m);
    void _reload(Model* m);
};

}

#endif // COMMANDLIST_H
