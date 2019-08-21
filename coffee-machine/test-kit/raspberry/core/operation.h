#ifndef CORE_OPERATION_H
#define CORE_OPERATION_H
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace core
{

class Operation
{
public:
    typedef boost::shared_ptr<Operation> pointer;
    virtual void run() = 0;
};

template <class T>
class Creator
{
public:
    virtual ~Creator(){}
    virtual boost::shared_ptr<T> create() = 0;
};

template <class OperationType>
class OperationCreator : public Creator<Operation>
{
public:
    Operation::pointer create()
    {
        return boost::make_shared<OperationType>();
    }
};

}

#endif // CORE_OPERATION_H
