#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <glog/logging.h>
#include "engine.h"

core::Engine* core::Engine::instance()
{
    static Engine* e = new Engine();
    return e;
}

boost::asio::io_service& core::Engine::getService()
{
    return io_service_;
}

core::Engine::Engine(): signals_(io_service_)
{
    this->thread_pool_size_ = 8;
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif
    signals_.async_wait(boost::bind(&core::Engine::stop, this));
}

void core::Engine::registerOperation(uint16_t type, boost::shared_ptr<Creator<Operation>> creator)
{
    operations_[type] = creator;
}

void core::Engine::start()
{
    for (auto& c: components_) c.second->init();
    //boost::asio::io_service::work work(io_service_);
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&core::Engine::execute_, this)));
        threads.push_back(thread);
    }

    for (auto& c: components_) c.second->start();
    for (std::size_t i = 0; i < threads.size(); ++i){threads[i]->join();}
}

void core::Engine::execute_()
{
    DLOG(INFO) << "[" << boost::this_thread::get_id() << "] Thread Start";

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service_.run( ec );
            if( ec )
            {
                DLOG(INFO)<< "[" << boost::this_thread::get_id()<< "] Error: " << ec;
            }
            break;
        }
        catch( std::exception & ex )
        {
            DLOG(INFO) << "[" << boost::this_thread::get_id()<< "] Exception: " << ex.what();
        }
    }

    DLOG(INFO) << "[" << boost::this_thread::get_id() <<"] Thread Finish";
}

void core::Engine::stop()
{
    for (auto& c: components_) c.second->stop();
    io_service_.stop();
}
