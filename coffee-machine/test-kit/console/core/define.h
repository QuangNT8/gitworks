#ifndef CORE_DEFINE_H
#define CORE_DEFINE_H

#define FIELD(type, name)\
public:\
    type name;

#define READONLY(type, name)\
    private:\
    Q_PROPERTY(type name READ get_##name NOTIFY name##Changed)\
    public:\
    type name;\
    type get_##name() const {return name;}\
    void set_##name(type name) { if(this->name != name) {this->name=name; emit name##Changed();}}\
    Q_SIGNAL void name##Changed();

#define PROPERTY(type, name)\
    private:\
    Q_PROPERTY(type name READ get_##name WRITE set_##name NOTIFY name##Changed)\
    public:\
    type name;\
    type get_##name() const {return name;}\
    void set_##name(type name) { if(this->name != name) {this->name=name; emit name##Changed();}}\
    Q_SIGNAL void name##Changed();

#define ENUM2(name)\
Q_ENUMS(name)\
public:\
    enum name

#define MODEL(name)\
class name: public core::Model\
{\
public:\
    name(QObject *parent = 0): core::Model(parent){}\
    virtual ~name(){}

#define END };

#endif
