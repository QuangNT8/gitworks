#ifndef FLATICON_H
#define FLATICON_H
#include "component.h"
#include "views.h"
#include "unordered_map"
#include <QChar>

COMPONENT(icon, Flaticon)
Q_OBJECT
public:
    void init()
    {
        this->setData_();
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
    void setData_();
    std::unordered_map<std::string, QChar> icons_;
COMPONENT_END

#endif
