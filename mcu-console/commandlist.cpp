#include "commandlist.h"
#include "core/model.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include<QRegularExpressionMatch>
using namespace std;

#define DATA_ROLE (Qt::UserRole+10)
console::CommandList::CommandList(QObject *parent): QAbstractListModel(parent)
{
    nextCommand_ = 1;
    filePath="";
}

void console::CommandList::copyFrom(CommandList* list)
{
    filePath = list->filePath;
    projectName = list->projectName;
    m_list.clear();
    nextCommand_ = 0;
    chart1_ = nullptr;
    chart2_ = nullptr;
    for(auto& m: list->data())
    {
        auto f = qobject_cast<FieldItem*>(m);
        if (f!=nullptr)
        {
            FieldItem* item = new FieldItem(this);
            item->type = f->type;
            item->name = f->name;
            item->min = f->min;
            item->max = f->max;
            item->value = f->value;
            item->command = f->command;
            item->readonly = f->readonly;
            m_list.append(item);
            if (nextCommand_ < f->command) nextCommand_ = f->command;
        }

        auto b = qobject_cast<ButtonItem*>(m);
        if (b!=nullptr)
        {
            ButtonItem* item = new ButtonItem(this);
            item->type = ITEM_BUTTON;
            item->name = b->name;
            item->command = b->command;
            m_list.append(item);
            if (nextCommand_ < b->command) nextCommand_ = b->command;
        }

        auto s = qobject_cast<SectionItem*>(m);
        if (s!=nullptr)
        {
            SectionItem* item = new SectionItem(this);
            item->type = ITEM_SECTION;
            item->name = s->name;
            m_list.append(item);
        }

        auto c = qobject_cast<ChartItem*>(m);
        if (c!=nullptr)
        {
            ChartItem* item = new ChartItem(this);
            item->type = ITEM_PLOT_HEADER;
            item->name = c->name;
            item->min = c->min;
            item->max = c->max;
            item->visible = c->visible;
            m_list.append(item);
            if (chart1_ == nullptr) chart1_ = item;
            else if (chart2_ == nullptr) chart2_ = item;
        }

        auto p = qobject_cast<PlotItem*>(m);
        if (p!=nullptr)
        {
            PlotItem* item = new PlotItem(this);
            item->type = ITEM_PLOT;
            item->name = p->name;
            item->color = p->color;
            item->highSpeed = p->highSpeed;
            m_list.append(item);
        }
    }
    nextCommand_++;
}

void console::CommandList::save()
{
    QFile fileSource(filePath);
    if (fileSource.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QTextStream stream(&fileSource);
        stream << "Project{name:'"<<projectName<<"'}\n";
        for (auto& m: m_list)
        {
            auto s = qobject_cast<SectionItem*>(m);
            if (s!=nullptr) stream<<"Section{name:'"<<s->name<<"'}\n";
            auto f = qobject_cast<FieldItem*>(m);
            if (f!=nullptr) stream<<"Field{name:'"<<f->name<<"',min:"<<f->min<<",max:"<<f->max<<",value:"<<f->value<<",readonly:"<<f->readonly<<",command:"<<f->command<<"}\n";
            auto b = qobject_cast<ButtonItem*>(m);
            if (b!=nullptr) stream<<"Button{name:'"<<b->name<<"',command:"<<b->command<<"}\n";
            auto c = qobject_cast<ChartItem*>(m);
            if (c!=nullptr) stream<<"Chart{ name:'"<<c->name<<"',min:"<<c->min<<",max:"<<c->max<<",visible:"<<c->visible<<"}\n";
            auto p = qobject_cast<PlotItem*>(m);
            if (p!=nullptr) stream<<"Plot{name:'"<<p->name<<"',color:'"<<p->color.name()<<"',highSpeed:"<<p->highSpeed<<"}\n";
        }
        QFileInfo tmp(filePath);
        set_folder(QUrl::fromLocalFile(tmp.path()));
    }
    fileSource.close();
    chart1_ = nullptr;
    chart2_ = nullptr;
    plotList_.clear();

    for (auto& m: m_list)
    {
        m->setParent(this);
        /*TODO: plot and chart*/
        auto c = qobject_cast<ChartItem*>(m);
        if (c != nullptr)
        {
            if (chart1_ == nullptr) chart1_ = c;
            else if (chart2_ == nullptr) chart2_ = c;
        }

        auto p = qobject_cast<PlotItem*>(m);
        if (p != nullptr)
        {
            plotList_.append(p);
            if (chart2_ != nullptr) p->chart = 1;
            else if (chart1_ != nullptr) p->chart = 0;
        }
    }
}

bool console::CommandList::load(QString fileName)
{
    static QRegularExpression rProject("^\\s*Project\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*}\\s*$");
    static QRegularExpression rField("^\\s*Field\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*,\\s*min\\s*:\\s*(-?\\d+)\\s*,\\s*max\\s*:\\s*(-?\\d+)\\s*,\\s*value\\s*:\\s*(-?\\d+)\\s*,\\s*readonly\\s*:\\s*(\\d)\\s*,\\s*command\\s*:\\s*(\\d+)\\s*}\\s*$");
    static QRegularExpression rButton("^\\s*Button\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*,\\s*command\\s*:\\s*(\\d+)\\s*}\\s*$");
    static QRegularExpression rSection("^\\s*Section\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*}\\s*$");
    static QRegularExpression rChart("^\\s*Chart\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*,\\s*min\\s*:\\s*(-?\\d+)\\s*,\\s*max\\s*:\\s*(-?\\d+)\\s*,\\s*visible\\s*:\\s*(\\d)\\s*}\\s*$");
    static QRegularExpression rPlot("^\\s*Plot\\s*{\\s*name\\s*:\\s*'([^']+)'\\s*,\\s*color\\s*:\\s*'([^']+)'\\s*,\\s*highSpeed\\s*:\\s*(\\d)\\s*}\\s*$");
    static QRegularExpression rEmpty("^\\s*$");

    beginResetModel();

    m_list.clear();
    projectName="untitled";
    filePath = fileName;
    chart1_ = nullptr;
    chart2_ = nullptr;

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
                //qDebug()<<"Project:"<< match.captured(1);
                projectName = match.captured(1);
                continue;
            }
            match = rField.match(line);
            if (match.hasMatch())
            {
                //qDebug()<<"Field:"<< match.captured(1);
                FieldItem* item = new FieldItem(this);
                item->type = ITEM_FIELD;
                item->name = match.captured(1);
                item->min = match.captured(2).toInt();
                item->max = match.captured(3).toInt();
                item->value = match.captured(4).toInt();
                item->readonly = (match.captured(5)=='1');
                item->command = match.captured(6).toInt();
                //qDebug()<<"command:"<< match.captured(5);
                m_list.append(item);
                continue;
            }
            match = rSection.match(line);
            if (match.hasMatch())
            {
                //qDebug()<<"Section:"<< match.captured(1);
                SectionItem* item = new SectionItem(this);
                item->type = ITEM_SECTION;
                item->name = match.captured(1);
                m_list.append(item);
                continue;
            }
            match = rButton.match(line);
            if (match.hasMatch())
            {
                //qDebug()<<"Button:"<< match.captured(1);
                ButtonItem* item = new ButtonItem(this);
                item->type = ITEM_BUTTON;
                item->name = match.captured(1);
                item->command = match.captured(2).toInt();
                m_list.append(item);
                continue;
            }
            match = rPlot.match(line);
            if (match.hasMatch())
            {
                //qDebug()<<"Plot:"<< match.captured(1);
                PlotItem* item = new PlotItem(this);
                item->type = ITEM_PLOT;
                item->name = match.captured(1);
                item->color = QColor(match.captured(2));
                item->highSpeed = (match.captured(3)=='1');
                if (chart2_ != nullptr) item->chart = 1;
                else if (chart1_ != nullptr) item->chart = 0;
                else return false;
                m_list.append(item);
                plotList_.append(item);

                continue;
            }
            match = rChart.match(line);
            if (match.hasMatch())
            {
                //qDebug()<<"Chart:"<< match.captured(1);
                ChartItem* item = new ChartItem(this);
                item->type = ITEM_PLOT_HEADER;
                item->name = match.captured(1);
                item->min = match.captured(2).toInt();
                item->max = match.captured(3).toInt();
                item->visible = (match.captured(4)=='1');
                if (chart1_ == nullptr)
                {
                    chart1_ = item;
                    item->index = 0;
                }
                else if (chart2_ == nullptr)
                {
                    chart2_ = item;
                    item->index = 1;
                }
                else return false;
                m_list.append(item);
                continue;
            }

            match = rEmpty.match(line);
            if (!match.hasMatch()) return false;

        }
        QFileInfo tmp(filePath);
        set_folder(QUrl::fromLocalFile(tmp.path()));
    }
    endResetModel();
    file.close();
    return true;
}

void console::CommandList::createNewModel()
{
    projectName = "Untitled";
    filePath="";
    chart1_ = new ChartItem(this);
    chart1_->type=ITEM_PLOT_HEADER;
    chart1_->name = "Chart 1";
    chart1_->index=0;
    chart1_->min= -4096;
    chart1_->max= 4096;
    chart1_->visible=true;
    add(chart1_);

    chart2_ = new ChartItem(this);
    chart2_->type=ITEM_PLOT_HEADER;
    chart2_->name = "Chart 2";
    chart2_->index = 1;
    chart2_->min= -4096;
    chart2_->max= 4096;
    chart2_->visible = true;
    add(chart2_);
    core::Model* sec2 = this->newSection("Fields");
    core::Model* sec1 = this->newSection("Actions");
    newField(sec2,"Field 3");
    newField(sec2,"Field 2");
    newField(sec2,"Field 1");
    newButton(sec1,"Button 2");
    newButton(sec1,"Button 1");
    newPlot(chart2_, "Plot 2");
    newPlot(chart1_, "Plot 1");
}

void console::CommandList::newField(Model* section,QString name)
{
    auto sIndex = m_list.indexOf(section);
    if (sIndex>=0)
    {
        FieldItem* item = new FieldItem(this);
        item->type = ITEM_FIELD;
        item->name = name;
        item->min = 0;
        item->max = 4096;
        item->value = 0;
        item->readonly = false;
        item->command = nextCommand_++;
        sIndex++;
        beginInsertRows(QModelIndex(), sIndex, sIndex);
        m_list.insert(sIndex, item);
        endInsertRows();
    }
}

void console::CommandList::newButton(Model* section,QString name)
{
    auto sIndex = m_list.indexOf(section);
    if (sIndex>=0)
    {
        ButtonItem* item = new ButtonItem(this);
        item->type=ITEM_BUTTON;
        item->name = name;
        item->command = nextCommand_++;
        sIndex++;
        beginInsertRows(QModelIndex(), sIndex, sIndex);
        m_list.insert(sIndex, item);
        endInsertRows();
    }
}

core::Model* console::CommandList::newSection(QString name)
{
    for(int i=0;i<size();i++)
    {
        ChartItem* chart1 = qobject_cast<ChartItem*>(m_list.at(i));
        if (chart1 !=nullptr)
        {
            SectionItem* item = new SectionItem(this);
            item->type= ITEM_SECTION;
            item->name = name;
            beginInsertRows(QModelIndex(), i, i);
            m_list.insert(i, item);
            endInsertRows();
            return item;
        }
    }
    return nullptr;
}

void console::CommandList::newPlot(Model* chart,QString name)
{
    auto sIndex = m_list.indexOf(chart);
    if (sIndex>=0)
    {
        PlotItem* item = new PlotItem(this);
        item->type=ITEM_PLOT;
        item->name = name;
        item->color = QColor("#444");
        item->highSpeed = false;
        sIndex++;
        beginInsertRows(QModelIndex(), sIndex, sIndex);
        m_list.insert(sIndex, item);
        endInsertRows();
    }
}

console::CommandList::~CommandList()
{
}

QHash<int, QByteArray> console::CommandList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DATA_ROLE] = "model";
    return roles;
}

int console::CommandList::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant console::CommandList::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == DATA_ROLE)
    {
        Model* m = m_list.at(index.row());
        return QVariant::fromValue<Model*>(m);
    }
    return QVariant();
}

const QList<core::Model*>& console::CommandList::data()
{
    return m_list;
}

int console::CommandList::size()
{
    return m_list.size();
}

void console::CommandList::reload(Model* m)
{
    QMetaObject::invokeMethod(this, "_reload", Qt::QueuedConnection,Q_ARG(Model*, m));
}

void console::CommandList::add(Model* m)
{
    QMutexLocker locker(&mutex);
    m->moveToThread(this->thread());
    //QMetaObject::invokeMethod(this, "_add", Qt::QueuedConnection,Q_ARG(Model*, m));
    _add(m);
}

void console::CommandList::addMulti(QList<Model*> list)
{
    QMutexLocker locker(&mutex);
    foreach (Model* m, list)
    {
        m->moveToThread(this->thread());
    }
    QMetaObject::invokeMethod(this, "_addMulti", Qt::QueuedConnection,Q_ARG(QList<Model*>, list));
}

void console::CommandList::remove(core::Model* m)
{
    QMutexLocker locker(&mutex);
    QMetaObject::invokeMethod(this, "_remove", Qt::QueuedConnection,Q_ARG(Model*, m));
}

void console::CommandList::_add(Model* m)
{
    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(m);
    endInsertRows();
}

void console::CommandList::_addMulti(QList<Model*> list)
{
    beginResetModel();
    foreach (Model* m, list)
    {
        m_list.append(m);
    }
    endResetModel();
}

void console::CommandList::_remove(Model* m)
{
    int index = m_list.indexOf(m);
    if(index != -1)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_list.removeAt(index);
        endRemoveRows();
    }
}

void console::CommandList::_reload(Model* m)
{
    int index = m_list.indexOf(m);
    if(index != -1) emit dataChanged(this->index(index), this->index(index));
}

void console::CommandList::moveUp(Model* m)
{
    int index = m_list.indexOf(m);
    if (index>0)
    {
        beginMoveRows(QModelIndex(),index, index,QModelIndex(), index-1);
        m_list.move(index, index-1);
        endMoveRows();
    }
}

void console::CommandList::moveDown(Model* m)
{
    int c1Index  = m_list.indexOf(chart1_);

    int index = m_list.indexOf(m);
    //qDebug()<<"chart1:"<<c1Index<<", i:"<<index;
    if (index < c1Index-1)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_list.removeAt(index);
        endRemoveRows();

        beginInsertRows(QModelIndex(), index+1, index+1);
        m_list.insert(index+1, m);
        endInsertRows();
    }
}

void console::CommandList::movePlotUp(Model* m)
{
    int c1Index  = m_list.indexOf(chart1_);
    int index = m_list.indexOf(m);
    if (index>c1Index+1)
    {
        beginMoveRows(QModelIndex(),index, index,QModelIndex(), index-1);
        m_list.move(index, index-1);
        endMoveRows();
    }
}

void console::CommandList::movePlotDown(Model* m)
{

    int index = m_list.indexOf(m);
    if (index < m_list.size()-1)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_list.removeAt(index);
        endRemoveRows();

        beginInsertRows(QModelIndex(), index+1, index+1);
        m_list.insert(index+1, m);
        endInsertRows();
    }
}


