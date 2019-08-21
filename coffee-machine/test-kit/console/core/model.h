#ifndef MODEL_H
#define MODEL_H

#include <QObject>

namespace core
{
#define MODEL(name)\
class name: public core::Model\
{\
public:\
    name(QObject *parent = 0): core::Model(parent){}\
    virtual ~name(){}

#define ENTITY(type, name)\
class name: public core::Model\
{\
protected:\
    type data_;\
    QMutex mutex_;\
public:\
    std::string id(){return data_.id();}\
    type& data(){return data_;}\
    name(const type &data, QObject *parent = 0): core::Model(parent), data_(data){}\
    virtual ~name(){}\
    void update(const type &data)\
    {\
        QMutexLocker locker(&mutex_);\
        data_.CopyFrom(data);\
    }\

#define END };

#define STRING_(name)\
    Q_PROPERTY(QString name READ get_##name NOTIFY name##Changed)\
    public:\
    Q_SIGNAL void name##Changed();\
    QString get_##name() const {return QString::fromStdString(data_.name());}\
    void set_##name(std::string val) {if (val!=data_.name()){data_.set_##name(val); emit name##Changed();}}\

#define UINT64_(name)\
    Q_PROPERTY(quint64 name READ get_##name NOTIFY name##Changed)\
    public:\
    Q_SIGNAL void name##Changed();\
    quint64 get_##name() const {return data_.name();}\
    void set_##name(quint64 val) {if (val!=data_.name()){;data_.set_##name(val); emit name##Changed();}}\

#define INT64_(name)\
    Q_PROPERTY(qint64 name READ get_##name NOTIFY name##Changed)\
    public:\
    Q_SIGNAL void name##Changed();\
    qint64 get_##name() const {return data_.name();}\
    void set_##name(qint64 val) {if (val!=data_.name()){;data_.set_##name(val); emit name##Changed();}}\

#define UINT32_(name)\
    Q_PROPERTY(quint32 name READ get_##name NOTIFY name##Changed)\
    public:\
    Q_SIGNAL void name##Changed();\
    quint32 get_##name() const {return data_.name();}\
    void set_##name(quint32 val) {if (val!=data_.name()){;data_.set_##name(val); emit name##Changed();}}\

#define BOOL_(name)\
    Q_PROPERTY(bool name READ get_##name NOTIFY name##Changed)\
    public:\
    Q_SIGNAL void name##Changed();\
    bool get_##name() const {return data_.name();}\
    void set_##name(bool val) {if (val!=data_.name()){;data_.set_##name(val); emit name##Changed();}}\

#define DOUBLE_(name)\
    Q_PROPERTY(double name READ get_##name NOTIFY name##Changed)\
    public:\
    Q_SIGNAL void name##Changed();\
    double get_##name() const {return data_.name();}\
    void set_##name(double val) {if (val!=data_.name()){;data_.set_##name(val); emit name##Changed();}}\

class Model: public QObject
{
Q_OBJECT
public:
    Q_INVOKABLE virtual std::string id(){return nullptr;}
    Model(QObject *parent = 0): QObject(parent){}
    virtual ~Model(){}
};

}
#endif // MODEL_H
