#ifndef CORE_CREATOR_H
#define CORE_CREATOR_H
#include <QObject>

namespace core
{

template <class T>
class Creator
{
public:
    virtual ~Creator(){}
    virtual T* create() = 0;
};

template <class Type>
class ObjectCreator : public Creator<QObject>
{
public:
    QObject* create(){return new Type;}
};

}
#endif // CREATOR_H
