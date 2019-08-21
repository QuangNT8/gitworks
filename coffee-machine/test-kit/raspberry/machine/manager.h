#ifndef KIT_MANAGER_H
#define KIT_MANAGER_H

#include <glog/logging.h>
#include "engine.h"
#include "tank.h"
#include "pump.h"
#include "tcp/connection.h"
#include "data.kit.pb.h"
#include "config.h"
#include "utils/utils.h"
#include <../../raspberry/ann/neuronnetwork.h>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define FLUSHING_TIME           10000/20
#define DELTA_FLUSHING_WEIGHT   5.0

#define MIN_TVALVE  5000
#define MAX_TVALVE  35000

#define MIN_VOLUME_OUT  50
#define MAX_VOLUME_OUT  350

#define MAX_WEIGHT              3000.0
#define FULL_OF_LIQUID          2000.0
#define OUT_OF_LIQUID           50.0

namespace kit
{

class Flusher
{
public:
    typedef boost::shared_ptr<Flusher> pointer;
    enum State {STOPPED = 0, FLUSHING, FLUSHED, ZERO_SETTING};
    void init(Tank::pointer tank, Pump::pointer pump);
    void start();

    void weightChanged(double weight, bool stable);
    void valveClosed();
private:
    void flush();

    Pump::pointer pump_;
    Tank::pointer tank_;
    State state_;

    double weight_;
    double old_weight_;
    double zero_offset_;
    bool stable_;
};

class Cleaner
{
public:
    typedef boost::shared_ptr<Cleaner> pointer;
    void init(Tank::pointer tank, Pump::pointer pump);
    void start();
    void stop();

    void weightChanged(double weight, bool stable);
    void valveClosed();
private:
    void fillLiquid() { pump_->turnOn(); }

    Pump::pointer pump_;
    Tank::pointer tank_;
};

class Sampler
{
public:
    typedef boost::shared_ptr<Sampler> pointer;
    enum State {STOPPED = 0, FILLING, SAMPLING, SAMPLED};
    void init(Tank::pointer tank, Pump::pointer pump);
    void start();
    void stop();

    void weightChanged(double weight, bool stable);
    void valveClosed();
private:
    void sample();

    Pump::pointer pump_;
    Tank::pointer tank_;
    State state_;

    double v_tank_;
    double v_out_;
    double v_out_expected_;
    uint16_t t_valve_;

    double weight_;
    bool stable_;
};

class Refiller
{
public:
    typedef boost::shared_ptr<Refiller> pointer;
    void init(Tank::pointer tank, Pump::pointer pump);
    void start();

    void weightChanged(double weight, bool stable);
private:
    Pump::pointer pump_;
    Tank::pointer tank_;
};

}

COMPONENT(kit, Manager)

public:
    void init();
    void start();

    enum State{STOPPED = 0, LIQUID_FLUSHING, TANK_CLEANING, SAMPLING, NETWORK_BUILDING, LIQUID_REFILLING};
    void serialReceived(uint16_t length, uint16_t type, uint8_t* data);
    bool ready() { return state_ == STOPPED; }
    State state() { return state_; }

    void flush(State next);
    void clean();
    void refill();
    void run();
    void stop();
    void build(std::string id);
    double predict(double v_out, std::string id = "");

    void weightChanged(double weight, bool stable);
    void valveOpened() { DLOG(INFO) << "valveOpened"; weight_open_valve_ = weight_; }
    void valveClosed();
    void pumpOpened() { DLOG(INFO) << "pumpOpened"; }
    void pumpClosed() { pump_->update(kit::Pump::CLOSED); }

    void flushingCompleted(double zero_offset);
    void cleaningCompleted();
    void buildingCompleted(data::kit::Project* project);
    void refillingCompleted();
    void sampleCreated(double v_tank, double v_out_expected, uint16_t t_valve, double v_out);

    tcp::Connection::pointer connection(){return connection_;}
    void setConnection(tcp::Connection::pointer connection) {this->connection_ = connection;}
    void pushToConsole(uint16_t type, const google::protobuf::Message &m);

    void load();
    void save();
    void changeProject(std::string id);
    std::string& projectId(){return project_id_;}
    data::kit::ProjectList& projects() {return projects_;}

    void learn(double v_tank, double v_out, uint16_t t_valve);

    Tank::pointer tank(){return tank_;}
private:
    State state_;
    State next_state_;
    void updateState(State state);
    void loadData();
    data::kit::Project* project();
    data::kit::File& data() {return data_;}

    std::unordered_map<std::string, ann::Network*> nets_;

    Tank::pointer tank_;
    Pump::pointer pump_;

    Flusher::pointer flusher_;
    Cleaner::pointer cleaner_;
    Sampler::pointer sampler_;
    Refiller::pointer refiller_;

    double weight_;
    double last_stable_weight_;
    double zero_offset_;
    double weight_open_valve_;

    tcp::Connection::pointer connection_;

    std::string project_id_ = "";
    data::kit::ProjectList projects_;
    data::kit::File data_;

    boost::mutex mutex_;

COMPONENT_END

#endif //KIT_MANAGER_H
