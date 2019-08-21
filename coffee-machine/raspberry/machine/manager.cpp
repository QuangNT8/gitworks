#include "manager.h"
#include "schedule.h"
#include "operation/operations.h"
#include "serial/controller.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#define BOARD_CONNECTION_LEFT "/dev/ttyUSB0"
#define BOARD_CONNECTION_RIGHT "/dev/ttyUSB1"
//#define BOARD_CONNECTION_LEFT "/dev/tty.SLAB_USBtoUART"
//#define BOARD_CONNECTION_RIGHT "/dev/tty.SLAB_USBtoUART2"

void bj::Manager::init()
{
    DLOG(INFO) << "Manager init";
    REGISTRATION(operations);

    auto controller = serial::Controller::instance();
    controller->setReceivedHandler(boost::bind(&bj::Manager::serialReceived, this, ::_1, ::_2,::_3,::_4));

    controller->addConnection(BOARD_CONNECTION_LEFT);
    controller->addConnection(BOARD_CONNECTION_RIGHT);

    auto left = controller->getConnection(BOARD_CONNECTION_LEFT);                   //FIXME: connection
    auto right = controller->getConnection(BOARD_CONNECTION_RIGHT);                 //FIXME: connection

    /*motors*/
    leftMotor_ = boost::make_shared<Motor>(left);
    rightMotor_ = boost::make_shared<Motor>(right);

    /*tanks init*/
    tanks_[0]  = boost::make_shared<Tank>(0);
    tanks_[1]  = boost::make_shared<Tank>(1);
    tanks_[2]  = boost::make_shared<Tank>(2);
    tanks_[3]  = boost::make_shared<Tank>(3);
    tanks_[4]  = boost::make_shared<Tank>(8);
    tanks_[5]  = boost::make_shared<Tank>(9);
    tanks_[6]  = boost::make_shared<Tank>(10);
    tanks_[7]  = boost::make_shared<Tank>(11);
    /* load data */
    int fd = open("data.pb", O_RDONLY);
    if (fd == -1) fd = open("build/default/install-root/data.pb", O_RDONLY);

    google::protobuf::io::FileInputStream fstream(fd);
    if (google::protobuf::TextFormat::Parse(&fstream, &data_))
    {
        /* calculate nextId */
        for (auto l: data_.liquids())
        {
            if (nextId_ < l.id()) nextId_ = l.id();
        }
        for (auto r: data_.recipes())
        {
            if (nextId_ < r.id()) nextId_ = r.id();
        }
        nextId_++;
        DLOG(INFO)<<"Data loaded, nextId = "<< nextId_;
    }
    else DLOG(INFO)<<"Error in reading data from data.pb";
    close(fd);
}

void bj::Manager::start()
{
    DLOG(INFO) << "Manager start";
    serial::Controller::instance()->startConnections();
    this->resetMachine();

    //Test set tank liquid
    tanks_[0]->change(2,2000);
    tanks_[4]->change(4,2000);
    tanks_[3]->change(7,2000);
    tanks_[7]->change(8,2000);
    tanks_[5]->change(1,2000);
    tanks_[6]->change(3,2000);

    tanks_[1]->change(5,2000);
    tanks_[2]->change(6,2000);
}
void bj::Manager::updateConnectionTank(serial::Connection::pointer con)
{
    for(int i = 0; i < TANK_COUNT; i++) tanks_[i]->updateConnection(con);
}
void bj::Manager::saveData()
{
    boost::mutex::scoped_lock lock(mutex_);
    int fd = open("data.pb", O_WRONLY);
    if (fd == -1) fd = open("build/default/install-root/data.pb", O_RDONLY);

    google::protobuf::io::FileOutputStream fstream(fd);
    if (google::protobuf::TextFormat::Print(data_, &fstream)) DLOG(INFO)<<"Data saved";
    else DLOG(INFO)<<"Error in saving data to data.pb";
}

ann::Network* bj::Manager::network(uint64_t id)
{
    boost::mutex::scoped_lock lock(mutex_);
    auto it = networks_.find(id);
    if (it!=networks_.end())
    {
        return it->second;
    }
    auto ret = buildNetwork_(id);

    if (ret!=nullptr) networks_[id] = ret;
    return ret;
}

void bj::Manager::updateNetwork(uint64_t id)
{
    boost::mutex::scoped_lock lock(mutex_);
    auto it = networks_.find(id);
    if (it!=networks_.end())
    {
        delete it->second;
        auto n = buildNetwork_(id);
        if (n!=nullptr) networks_[id] = n;
    }
}

ann::Network* bj::Manager::buildNetwork_(uint64_t id)
{
    //FIXME: trained offline
    auto ann = ann::Network::load("trained_model.pb");
    if (ann == nullptr) ann = ann::Network::load("build/default/install-root/trained_model.pb");
    return ann;

    for (auto l: data_.liquids())
    {
        if (l.id() == id)
        {
            DLOG(INFO) << "load neural network of liquid " << id;
            if(l.has_network())
            {
                return ann::Network::load(l.network());
            }
            else
            {
                return nullptr;
            }
        }
    }
    return nullptr;
}

void bj::Manager::setConsoleConnection(tcp::Connection::pointer connection)
{
    this->consoleConnection_ = connection;
}

void bj::Manager::pushToConsole(uint16_t type, const google::protobuf::Message &m)
{
    if (consoleConnection_ != nullptr)
    {
        consoleConnection_->sendMessage(0, type, m);
    }
}

void bj::Manager::pushToOrder(uint64_t sessionId, uint16_t type, const google::protobuf::Message &m)
{
    auto session = getSession(sessionId);
    if (session != nullptr)
    {
        session->sendMessage(0, type, m);
    }
}

void bj::Manager::resetMachine()
{
    machineState_ = NOT_READY;

    comm::machine::StateChanged state;
    state.set_machineready(this->machineReady());
    this->pushToConsole(comm::machine::StateChanged::kEXTFieldNumber, state);

    leftMotor_->reset();
    rightMotor_->reset();
    bj::Schedule::instance()->reset();
}

void bj::Manager::changeDebugMode(bool value)
{
    DLOG(INFO) << "changeDebugMode";
    if (this->debugMode_ != value)
    {
//        if(value && bj::Schedule::instance()->occupied())
//        {
//            comm::machine::MessageReceived event;
//            event.set_text("A door is making drink !");
//            bj::Manager::instance()->pushToConsole(comm::machine::MessageReceived::kEXTFieldNumber, event);
//            return;
//        }
//        DLOG(INFO) << "Change to " << (value ? "debug" : "normal") << " mode!";

        this->setMode_ = value;
        this->resetMachine();
    }
}

tcp::Connection::pointer bj::Manager::getSession(uint64_t id)
{
    boost::mutex::scoped_lock lock(sessionMutex_);
    auto it = sessions_.find(id);
    if (it != sessions_.end()) return it->second;
    return nullptr;
}

void bj::Manager::setSession(uint64_t id, tcp::Connection::pointer con)
{
    boost::mutex::scoped_lock lock(sessionMutex_);
    auto it = sessions_.find(id);
    if (it == sessions_.end())
    {
        sessions_[id] = con;
    }
}
