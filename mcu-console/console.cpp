#include "console.h"
#include "core/views.h"
#include "serial/controller.h"
#include "type.h"
#include <QXYSeries>
#include <QQuickWindow>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QDir>
#include "serial/controller.h"
using namespace QtCharts;

void console::Controller::init()
{
    qmlRegisterInterface<console::CommandList>("CommandList");
    commands = new console::CommandList(this);
    core::Views::instance()->setPath("qrc:/main.qml");
    core::Views::instance()->setWidth(1500);
    core::Views::instance()->setHeight(850);
    core::Views::instance()->registerComponent("Console", this);
    itemcount_=0;
    pause_ = false;

    recentList = new console::RecentList(this);
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    //qDebug()<<path;
    if (!path.isEmpty()) recentList->load(path+"/setting.txt");
}

bool console::Controller::open(QString fileName)
{
    CommandList* list = new CommandList(this);
    if (list->load(fileName))
    {
        swapModel(list);
        return true;
    }
    return false;
}

void console::Controller::initCharts_()
{
    QMetaObject::invokeMethod(core::Views::instance()->getWindow(), "initSeries", Qt::DirectConnection);
    for(auto& p: commands->plots())
    {
        QVariant ret;
        QMetaObject::invokeMethod(core::Views::instance()->getWindow(), "addSeries", Qt::DirectConnection,Q_RETURN_ARG(QVariant, ret),Q_ARG(QVariant, p->chart),Q_ARG(QVariant, p->name));
        QXYSeries* s = qvariant_cast<QXYSeries*>(ret);
        if (s!=nullptr)
        {
            p->plot = s;
            s->setColor(p->color);

            if (p->highSpeed)
            {
                OscLine* line = new OscLine(s);
                p->line = line;
            }
            else
            {
                PlotLine* line = new PlotLine(s);
                p->line = line;
            }
        }
    }

    if (commands->chart1()!=nullptr)
    {
        setYAxisRange(0, commands->chart1()->min, commands->chart1()->max);
    }

    if (commands->chart2()!=nullptr)
    {
        setYAxisRange(1, commands->chart2()->min, commands->chart2()->max);
    }
    itemcount_ = 0;

    recentList->add(commands->projectName, commands->filePath);
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (!path.isEmpty())
    {
        QDir dir(path);

        if (!dir.exists()) dir.mkpath(path);
        recentList->save(path+"/setting.txt");
    }

}
void console::Controller::appendPlot(uint8_t plot, uint16_t index, int16_t v)
{
    auto p = commands->plots().at(plot);
    if (p!=nullptr)
    {
        p->line->addValue(index, v);
    }
}

void console::Controller::upadateField(uint16_t cmd, int16_t val)
{
    auto list = commands->data();

    for (auto & m: list)
    {
        auto f = qobject_cast<FieldItem*>(m);
        if (f!=nullptr)
        {
            if (f->command == cmd)
            {
                f->set_value(val);
                return;
            }
        }
    }
}

void console::Controller::displayOsc(uint8_t* data, uint16_t length)
{
    uint8_t plot = data[0];
    if (plot >= commands->plots().size()) return;
    auto p = this->commands->plots().at(plot);
    if (p == nullptr) return;
    if (!p->highSpeed) return;
    uint16_t index = ((uint16_t)data[1]<<8) + (uint16_t)data[2];
    int16_t base = ((int16_t)data[3]<<8) + (int16_t)data[4];

    p->line->addValue(index++, base);

    for (int i = 5;i<length;i++)
    {
        base += (int8_t)data[i];
        p->line->addValue(index++, base);
    }
    if (index > 1000 && !pause_) p->line->flush(0);
}

void console::Controller::displayPlot(uint8_t* data, uint16_t length)
{
    uint16_t mask = data[0];
    bool needRemove = false;
    itemcount_++;
    if (itemcount_>10)
    {
        needRemove = true;
        itemcount_ = 0;
    }
    currentIndex_++;
    int index = 1;
    auto list = commands->plots();

    for (auto& p: commands->plots())
    {
        if (index > length - 2) break;

        if ((mask & 1) == 1 )
        {
            int16_t val = ((int16_t)data[index++]<<8) + data[index++];
            p->line->addValue(currentIndex_, val);
            if (!pause_ && needRemove)
            {
                if (currentIndex_<1000)p->line->flush(0);
                else p->line->flush(currentIndex_-1000);
            }
        }
        mask >>= 1;
    }

    serial::Controller::instance()->incFreq();
}



void console::Controller::setVariable(uint16_t command, QString v)
{
    bool ok = false;
    int32_t tmp = v.toInt(&ok);

    if (ok)
    {
        uint8_t buf[4];
        buf[0] = (tmp >> 24 ) & 0xFF;
        buf[1] = (tmp >> 16 ) & 0xFF;
        buf[2] = (tmp >> 8 ) & 0xFF;
        buf[3] = (tmp & 0xFF);
        serial::Controller::instance()->send(command, 4, buf);
    }
    else
    {
        core::Views::instance()->showMessage("Invalid value:" + v);
    }
}

void console::Controller::buttonClicked(uint16_t command)
{
    serial::Controller::instance()->send(command, 0, nullptr);
}

console::CommandList* console::Controller::editModel()
{
    console::CommandList* ret = new console::CommandList(this);
    if (commands->size()>0) ret->copyFrom(commands);
    else ret->createNewModel();
    return ret;
}

void console::Controller::close()
{
    commands->deleteLater();
    commands = new console::CommandList(this);
}

void console::Controller::swapModel(CommandList* model)
{
    auto tmp = commands;
    set_commands(model);
    initCharts_();
    tmp->deleteLater();
}

QString console::Controller::generateHeaderFile(QUrl folder)
{
    QString funcs;
    QString decls;
    QString ret;
    QString readOnly;
    QString readOnly_var;
    QString readOnly_loop;

    //qDebug()<<folder.toLocalFile();

    const auto list = commands->data();
    for (const auto& m: list)
    {
        QString name;
        auto f = qobject_cast<FieldItem*>(m);
        if (f!=nullptr)
        {
            name = f->name.simplified().toLower().replace(' ','_');
            if (f->readonly)
            {
                readOnly.append("\tvoid update_").append(name).append("(int16_t v){").
                        append(name).append("=v;").append(name).append("_ready=true;}\n");
                readOnly_var.append("\tint16_t ").append(name).append(";bool ").append(name).append("_ready = false;\n");
                readOnly_loop.append("\t\tif (").append(name).append("_ready && uart::CONTROLLER.updateField(").
                        append(QString::number(f->command)).append(",").append(name).append(")) ").
                        append(name).append("_ready = false;\n");
            }
            else
            {

                funcs.append("UART_COMMAND(").append(name)
                        .append(")\n{\n\tint32_t val = DECODE_VAL;\n\t")
                        .append("uart::CONTROLLER.printfMessage(\"Field ").append(name).append(" = %d\",val);\n\t/*TODO: implement*/\n}\n\n");
                decls.append("\tCOMMAND_REG(").append(QString::number(f->command)).append(", ").append(name).append(")\n");
            }
        }

        auto b = qobject_cast<ButtonItem*>(m);
        if (b!=nullptr)
        {
            name = b->name.simplified().toLower().replace(' ','_');
            funcs.append("UART_COMMAND(").append(name).append(")\n{\n\tuart::CONTROLLER.printMessage(\"Button '").append(b->name).append("' clicked\");\n\t/*TODO: implement*/\n}\n\n");
            decls.append("\tCOMMAND_REG(").append(QString::number(b->command)).append(", ").append(name).append(")\n");
        }
    }
    ret.append("#include \"command.h\"\n#include \"uart/controller.h\"\n#define DECODE_VAL ((int32_t)data[0] <<24)| ((int32_t)data[1] << 16)| ((int32_t)data[2] <<8) | (int32_t)data[3]\n\n").append(funcs).append("plot::Controller plot::CONTROLLER;\nUART_COMMAND_BEGIN\n").append(decls).append("UART_COMMAND_END");

    QFile fileSource(folder.toLocalFile()+"/command.cpp");
    if (fileSource.exists())
    {
        QString backupName = folder.toLocalFile()+"/command_" + QString::number(QDateTime::currentMSecsSinceEpoch())+".cpp";
        fileSource.rename(backupName);
    }
    fileSource.setFileName(folder.toLocalFile()+"/command.cpp");
    if (fileSource.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QTextStream stream(&fileSource);
        stream <<ret;
    }
    fileSource.close();

    QString methods;
    QString flush;
    QString vars;
    QString setup;
    QString loop1;

    const auto plotList = commands->plots();
    int i=0;
    for (const auto& p: plotList)
    {
        QString name = p->name.simplified().toLower().replace(' ','_');
        if (p->highSpeed)
        {
            methods.append("\tinline uint16_t plot_").append(name).append("(int16_t v){")
                    .append("return ").append(name).append("_.addValue(v);}\n")
                    .append("\tinline void plot_").append(name).append("(int16_t v, uint16_t s){")
                    .append(name).append("_.addValue2(v,s);}\n")
                    .append("\tinline void set_").append(name).append("_threshold(int16_t t){").append(name).append("_.setThreshold(t);}\n");

            vars.append("\tOscilloscope ").append(name).append("_;\n");
            setup.append("\t\t").append(name).append("_.setup(").append(QString::number(i)).append(");\n");
            loop1.append("\t\tdone &= ").append(name).append("_.loop(done_);\n");
        }
        else
        {
            methods.append("\tvoid plot_").append(name).append("(int16_t v)\n\t{\n\t\t").append(name).append("_= v;\n\t\t_mask |= ").append(QString::number((1<<i))).append(";\n\t}\n");
            flush.append("\t\tif (_mask & ")
                    .append(QString::number((1<<i)))
                    .append(")\n\t\t{\n\t\t\t_data[dataLen_++] = ((")
                    .append(name).append("_ >> 8) & 0xFF);\n\t\t\t_data[dataLen_++] = (")
                    .append(name).append("_ & 0xFF);\n\t\t}\n");
            vars.append("\tint16_t ").append(name).append("_;\n");
        }
        i++;
    }

    QFile fileHeader(folder.toLocalFile()+"/command.h");
    if (fileHeader.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QTextStream stream(&fileHeader);
        stream << "#ifndef MCUD_COMMAND_H\n#define MCUD_COMMAND_H\n#include \"uart/controller.h\"\n";
        stream << "#include\"uart/oscilloscope.h\"\n\n";
        stream << "namespace plot\n{\n\nclass Controller\n{\npublic:\n\tController(): _mask(0){}\n";
        stream << "void init()\n\t{\n"<<setup<<"\t\tneedFlush_ = false;\n\t}\n";
        stream<<methods;
        stream<<readOnly;
        stream<<"\tvoid loop()\n\t{\n\t\tbool done = true;\n"<<loop1;
        stream<<"\t\tdone_ = done;\n\t\tif (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;\n";
        stream<<readOnly_loop<<"\n\t}\n";
        stream<<"\n\tvoid flush()\n\t{\n\t\tif (_mask==0) return;\n\t\tdataLen_ = 0;\n\t\t_data[dataLen_++] = _mask;\n";
        stream<<flush;
        stream<<"\t\tneedFlush_ = true;\n\t\t_mask = 0;\n\t}\nprivate:\n\tuint8_t _mask;\n\tuint8_t _data["<<QString::number(i*2+2)<<"];\n";
        stream<<"\tbool done_;\n";
        stream<<"\tbool needFlush_;\n";
        stream<<"\tuint8_t dataLen_;\n";
        stream<<readOnly_var;
        stream<<vars<<"};\n\nextern plot::Controller CONTROLLER;\n\n}\n\n#endif ";
    }
    fileHeader.close();

    return "";
}

void console::Controller::setYAxisRange(uint16_t chart, int32_t min, int32_t max)
{
    QMetaObject::invokeMethod(core::Views::instance()->getWindow(), "setYAxisRange", Qt::DirectConnection,Q_ARG(QVariant, chart),Q_ARG(QVariant, min),Q_ARG(QVariant, max));
}

void console::Controller::setXAxisRange(uint16_t chart, int32_t min, int32_t max)
{
    QMetaObject::invokeMethod(core::Views::instance()->getWindow(), "setXAxisRange", Qt::DirectConnection,Q_ARG(QVariant, chart),Q_ARG(QVariant, min),Q_ARG(QVariant, max));
}

QString console::Controller::convertUrl(QUrl url)
{
    return url.toLocalFile();
}
