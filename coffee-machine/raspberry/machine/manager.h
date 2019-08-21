#ifndef BJ_MANAGER_H
#define BJ_MANAGER_H

#define TANK_COUNT 8
#define POSITION_UPDATE_CYCLE 50   //ms
#include <glog/logging.h>
#include "engine.h"
#include "motor.h"
#include "tank.h"
#include "data.bj.pb.h"
#include "tcp/connection.h"
#include "config.h"

COMPONENT(bj, Manager)

public:
    void init();
    void start();

    void saveData();
    data::bj::Data& data(){return data_;}
    ann::Network* network(uint64_t id);
    void updateNetwork(uint64_t id);

    tcp::Connection::pointer getConsoleConnection(){return consoleConnection_;}
    void setConsoleConnection(tcp::Connection::pointer connection);
    void pushToConsole(uint16_t type, const google::protobuf::Message &m);
    void pushToOrder(uint64_t sessionId, uint16_t type, const google::protobuf::Message &m);

    uint64_t nextId()
    {
        boost::mutex::scoped_lock lock(mutex_);
        return nextId_++;
    }
    Tank::pointer getTank(uint32_t index) {return tanks_[index];}
    Motor::pointer leftMotor() {return this->leftMotor_;}
    Motor::pointer rightMotor() {return this->rightMotor_;}
    void resetMachine();

    void changeDebugMode(bool value);
    bool debugMode() { return debugMode_; }
    bool machineReady() { return (machineState_ == READY); }
    void updateConnectionTank(serial::Connection::pointer con);

    //Test run forever
     std::vector<std::pair<uint64_t, double>> ingredientsLeft;
     std::vector<std::pair<uint64_t, double>> ingredientsRight;

    //Session
    tcp::Connection::pointer getSession(uint64_t id);
    void setSession(uint64_t id, tcp::Connection::pointer con);
    uint64_t sessionId()
    {
        boost::mutex::scoped_lock lock(mutex_);
        return sessionId_++;
    }
private:
    Motor::pointer leftMotor_;
    Motor::pointer rightMotor_;
    Tank::pointer tanks_[TANK_COUNT];
    data::bj::Data data_;
    std::unordered_map<uint64_t, ann::Network*> networks_;
    ann::Network* buildNetwork_(uint64_t id);

    tcp::Connection::pointer consoleConnection_;
    std::unordered_map<uint64_t, tcp::Connection::pointer> sessions_;
    boost::mutex sessionMutex_;
    uint64_t sessionId_ = 1000;

    uint64_t nextId_ = 100;
    boost::mutex mutex_;
    boost::mutex stateReadyMutex_;
    void serialReceived(uint16_t length, uint16_t type, uint8_t* data, serial::Connection::pointer con);

    enum State{NOT_READY=1, LEFT_READY, RIGHT_READY, READY};
    uint16_t machineState_ = NOT_READY;
    bool machineStart = true;

    bool debugMode_ = false;
    bool setMode_ = false;
COMPONENT_END

#endif //MANAGER_H
