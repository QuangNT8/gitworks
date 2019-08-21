#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <typeindex>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "component.h"
#include "operation.h"
#include <boost/make_shared.hpp>

namespace core
{

class Engine
{
public:
    template<class C>
    void registerComponent()
    {
        auto t = std::type_index(typeid(C));
        components_[t] = boost::make_shared<C>();
    }

    template<class C>
    boost::shared_ptr<C> getComponent()
    {
        auto t = std::type_index(typeid(C));
        return boost::dynamic_pointer_cast<C>(components_[t]);
    }

    void start();
    void stop();
    void registerOperation(uint16_t type, boost::shared_ptr<Creator<Operation>> creator);

    template<class O>
    boost::shared_ptr<O> createOperation(uint16_t type)
    {
        auto it = operations_.find(type);
        if (it!=operations_.end())
        {
            return  boost::dynamic_pointer_cast<O>(it->second->create());
        }
        return nullptr;
    }

    static Engine* instance();
    boost::asio::io_service& getService();
private:
    Engine();
    void execute_();

    boost::asio::io_service io_service_;
    std::size_t thread_pool_size_;
    boost::asio::signal_set signals_;
    std::unordered_map<std::type_index, boost::shared_ptr<Component>> components_;
    std::unordered_map<uint16_t, boost::shared_ptr<Creator<Operation>>> operations_;
};

}

#endif // ENGINE_H
