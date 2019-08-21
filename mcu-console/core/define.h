#ifndef DEFINE_H
#define DEFINE_H

#define FIELD(type, name)\
public:\
    type name;

#define READONLY(type, name)\
    Q_PROPERTY(type name READ get_##name CONSTANT)\
    public:\
    type name;\
    type get_##name() const {return name;}

#define PROPERTY(type, name)\
    Q_PROPERTY(type name READ get_##name WRITE set_##name NOTIFY name##Changed)\
    public:\
    type name;\
    type get_##name() const {return name;}\
    void set_##name(type name) { if(this->name != name) {this->name=name; emit name##Changed();}}\
    Q_SIGNAL void name##Changed();

#define ENUM(name)\
Q_ENUMS(name)\
public:\
    enum name

#define EVENT(func)\
public:\
    Q_SIGNAL void func;

#define MODEL(name)\
class name: public core::Model\
{\
public:\
    name(QObject *parent = 0): core::Model(parent){}\
    virtual ~name(){}

#define END };

/*-------------------------------------------------------------------------------------------------------*/
#define SIZE(name, value)\
    Q_PROPERTY(int name READ get_##name CONSTANT)\
    public:\
    int name = value*RATIO;\
    int get_##name() const {return name;}

#define ICON(name, value)\
    Q_PROPERTY(QString name READ get_##name CONSTANT)\
    public:\
    QString name = value;\
    QString get_##name() const {return name;}

#define COLOR(name, value)\
    Q_PROPERTY(QColor name READ get_##name CONSTANT)\
    public:\
    QColor name = QColor(value);\
    QColor get_##name() const {return name;}

#endif // DEFINE_H
