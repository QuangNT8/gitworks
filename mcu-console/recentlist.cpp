#include "recentlist.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include <QDebug>

#define DATA_ROLE (Qt::UserRole+10)

console::RecentList::RecentList(QObject *parent): QAbstractListModel(parent)
{

}

console::RecentList::~RecentList()
{

}

void console::RecentList::add(QString name, QString file)
{
    //qDebug()<<"1.-1";
    beginResetModel();
    //qDebug()<<"1.0";
    for (auto& m: m_list)
    {
        //qDebug()<<"1.0.1";
        if (m->file == file)
        {
            //qDebug()<<"1.1";
            if (m_list.removeOne(m)) {/*m->deleteLater();*/}
            //qDebug()<<"1.2";
        }
    }

    //qDebug()<<"1.3";
    RecentItem* item = new RecentItem(this);
    item->name = name;
    item->file = file;

    m_list.prepend(item);
    //qDebug()<<"1.4";
    while (m_list.size()>10)
    {
        auto t = m_list.last();
        m_list.removeLast();
        t->deleteLater();
    }
    endResetModel();
}

void console::RecentList::save(QString fileName)
{
    QFile fileSource(fileName);
    if (fileSource.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QTextStream stream(&fileSource);
        for (auto& m: m_list)
        {
            stream<<"Project{name:'"<<m->name<<"',file:'"<<m->file<<"'}\n";
        }
    }
    else
    {
        qDebug()<<"Can not write setting file ";
    }
    fileSource.close();
}

void console::RecentList::load(QString fileName)
{
    static QRegularExpression rProject("^\\s*Project\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*,\\s*file\\s*:\\s*'([^']+)'\\s*}\\s*$");

    beginResetModel();
    m_list.clear();

    QRegularExpressionMatch match;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            match = rProject.match(line);
            if (match.hasMatch())
            {
                qDebug()<<"Project:"<< match.captured(1);
                RecentItem* item = new RecentItem(this);
                item->name = match.captured(1);
                item->file = match.captured(2);
                m_list.append(item);
            }
        }
    }
    endResetModel();
    file.close();
}

QHash<int, QByteArray> console::RecentList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DATA_ROLE] = "model";
    return roles;
}

int console::RecentList::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant console::RecentList::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == DATA_ROLE)
    {
        core::Model* m = m_list.at(index.row());
        return QVariant::fromValue<core::Model*>(m);
    }
    return QVariant();
}
