#ifndef FLATICON_H
#define FLATICON_H
#include "component.h"
#include "views.h"
#include "unordered_map"
#include <QChar>
#include <QFile>
#include <QTextStream>

COMPONENT(icon, Flaticon)
Q_OBJECT
public:
    void init()
    {
        QRegExp re("^\\.flaticon-([^:]*):before\\s\\{\\scontent:\\s\"\\\\(.*)\";\\s\\}$");
        QFile inputFile("../install-root/flaticon.css");
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (re.exactMatch(line))
                {
                    bool ok;
                    short code = re.cap(2).toUpper().toUShort(&ok, 16);
                    if (ok)
                    {
                        icons_[re.cap(1).toStdString()] = QChar(code);
                    }
                }
            }
            inputFile.close();
        }
        else qDebug()<<"can not ope css file";
        core::Views::instance()->registerComponent("Flaticon", this);
    }
    QChar get(std::string s)
    {
        auto it = icons_.find(s);
        if (it!=icons_.end()) return it->second;
        return ' ';
    }
    Q_INVOKABLE QChar get(QString s)
    {
        auto it = icons_.find(s.toStdString());
        if (it!=icons_.end()) return it->second;
        return ' ';
    }
private:
    std::unordered_map<std::string, QChar> icons_;
COMPONENT_END

#endif
