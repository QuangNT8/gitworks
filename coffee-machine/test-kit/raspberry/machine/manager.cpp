#include "manager.h"
#include "operation/operations.h"
#include "serial/controller.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#define BOARD_CONNECTION "/dev/ttyUSB0"
//#define BOARD_CONNECTION "/dev/tty.SLAB_USBtoUART"

void kit::Manager::init()
{
    DLOG(INFO) << "Manager init";
    REGISTRATION(operations);

    auto controller = serial::Controller::instance();
    controller->addConnection(BOARD_CONNECTION);

    tank_  = boost::make_shared<Tank>(controller->getConnection(BOARD_CONNECTION));
    pump_  = boost::make_shared<Pump>(controller->getConnection(BOARD_CONNECTION));

    //init
    flusher_ = boost::make_shared<Flusher>();
    cleaner_ = boost::make_shared<Cleaner>();
    sampler_ = boost::make_shared<Sampler>();
    refiller_ = boost::make_shared<Refiller>();

    flusher_->init(tank_, pump_);
    cleaner_->init(tank_, pump_);
    sampler_->init(tank_, pump_);
    refiller_->init(tank_, pump_);

    weight_ = MAX_WEIGHT;
    state_ = STOPPED;

    controller->setReceivedHandler(boost::bind(&kit::Manager::serialReceived, this, ::_1, ::_2,::_3));
}

void kit::Manager::start()
{
    DLOG(INFO) << "Manager start";
    serial::Controller::instance()->startConnections();

    load();

    this->flush(STOPPED);
}

void kit::Manager::flush(State next)
{
    DLOG(INFO) << "Flush liquid";
    if(state_ != STOPPED) DLOG(INFO) << "The system is still working. Please stop it before call flush!";
    else
    {
        next_state_ = next;
        updateState(LIQUID_FLUSHING);
        flusher_->start();
    }
}

void kit::Manager::refill()
{
    DLOG(INFO) << "Refill liquid";
    if(state_ != STOPPED) DLOG(INFO) << "The system is still working. Please stop it before call refill!";
    else
    {
        updateState(LIQUID_REFILLING);
        refiller_->start();
    }
}

void kit::Manager::clean()
{
    DLOG(INFO) << "Clean tank";
    if(state_ != STOPPED) DLOG(INFO) << "The system is still working. Please stop it before call clean!";
    else this->flush(TANK_CLEANING);
}

void kit::Manager::run()
{
    DLOG(INFO) << "Start sampling";
    if(state_ != STOPPED) DLOG(INFO) << "The system is still running!";
    else
    {
        if (this->project()->remain() <= 0)
        {
            DLOG(INFO) << "Sampling finished";
            comm::kit::MessageReceived event;
            event.set_text("Sampling finished");
            Manager::instance()->pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
        }
        else
        {
            DLOG(INFO) << "OK";
            this->flush(SAMPLING);
        }
    }
}

void kit::Manager::stop()
{
    DLOG(INFO) << "Stop";
    if(state_ == STOPPED) DLOG(INFO) << "The system is still stopped!";
    else
    {
        DLOG(INFO) << "OK";
        switch (state_)
        {
            case SAMPLING:
                sampler_->stop();
                this->save();
                this->project_id_ = "";
                // send stopped event
//                comm::kit::MessageReceived event;
//                event.set_text("Sampling Stopped");
//                pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
                break;
            case TANK_CLEANING:
                cleaner_->stop();
                break;
            default:
                DLOG(INFO) << "The system is still working. Cannot stop!";
                break;
        }

        updateState(STOPPED);
    }
}


void kit::Manager::weightChanged(double weight, bool stable)
{
    weight_ = weight - zero_offset_;
    if(abs(last_stable_weight_ - weight_) > 10.0 && stable)
    {
        DLOG(INFO) << "weightChanged weight " << weight << ", weight_ " << weight_ << ", (weight_open_valve_ - weight_) = " << weight_open_valve_ - weight_;
        last_stable_weight_ = weight_;
    }

    switch (state_)
    {
    case LIQUID_REFILLING:
        refiller_->weightChanged(weight_, stable);
        break;
    case LIQUID_FLUSHING:
        flusher_->weightChanged(weight, stable);
        break;
    case TANK_CLEANING:
        cleaner_->weightChanged(weight_, stable);
        break;
    case SAMPLING:
        sampler_->weightChanged(weight_, stable);
        break;
    default:
        break;
    }
}

void kit::Manager::valveClosed()
{
    DLOG(INFO) << "valveClosed";

    switch (state_)
    {
    case LIQUID_FLUSHING:
        flusher_->valveClosed();
        break;
    case TANK_CLEANING:
        cleaner_->valveClosed();
        break;
    case SAMPLING:
        sampler_->valveClosed();
        break;
    default:
        break;
    }
}

void kit::Manager::flushingCompleted(double zero_offset)
{
    zero_offset_ = zero_offset;
    if(next_state_ != STOPPED)
    {
        switch (next_state_)
        {
            case LIQUID_REFILLING:
                updateState(LIQUID_REFILLING);
                refiller_->start();
                break;
            case TANK_CLEANING:
                updateState(TANK_CLEANING);
                cleaner_->start();
                break;
            case SAMPLING:
                {
                    // send started event
                    comm::kit::MessageReceived event;
                    event.set_text("Sampling started");
                    Manager::instance()->pushToConsole(comm::kit::MessageReceived::kEXTFieldNumber, event);
                    updateState(SAMPLING);
                    sampler_->start();
                }
                break;
            default:
                break;
        }
    }
    else updateState(STOPPED);
}

void kit::Manager::cleaningCompleted()
{
    updateState(STOPPED);
}

void kit::Manager::refillingCompleted()
{
    updateState(STOPPED);
}

void kit::Manager::sampleCreated(double v_tank, double v_out_expected, uint16_t t_valve, double v_out)
{
    DLOG(INFO) << "Sample created: v_tank " << v_tank << " v_out_expected " << v_out_expected
               << " t_valve " << t_valve * 20 << "ms v_out " << v_out
               << " error " << 100.0*(v_out - v_out_expected)/v_out_expected << "%";

    if(v_tank - v_out < OUT_OF_LIQUID)
    {
        DLOG(INFO) << "Ignore last sample";
        return;
    }

    auto s = this->data().mutable_samples()->Add();
    s->set_time(t_valve);
    s->set_vtank(v_tank);
    s->set_vout(v_out);
    s->set_index(static_cast<uint32_t>(this->data().samples_size()));
    s->set_vexpected(v_out_expected);
    s->set_datetime(utils::getDateTime());

    uint32_t remain = this->project()->total() - static_cast<uint32_t>(this->data().samples_size());
    this->project()->set_remain(remain);

    if(remain % 20 == 0) this->save();

    comm::kit::SampleCreated event;

    event.set_project(this->project()->id());
    event.set_remain(this->project()->remain());

    event.mutable_sample()->set_time(s->time());
    event.mutable_sample()->set_vtank(s->vtank());
    event.mutable_sample()->set_vout(s->vout());
    event.mutable_sample()->set_index(s->index());
    event.mutable_sample()->set_vexpected(s->vexpected());

    pushToConsole(comm::kit::SampleCreated::kEXTFieldNumber, event);

    learn(v_tank, v_out, t_valve);

    if(remain <= 0)
    {
        sampler_->stop();
        updateState(STOPPED);
    }
}

void kit::Manager::buildingCompleted(data::kit::Project* project)
{
    DLOG(INFO) << "Build network done";
    updateState(STOPPED);

    comm::kit::ProjectUpdated event;
    event.mutable_project()->set_id(project->id());
    event.mutable_project()->set_name(project->name());
    event.mutable_project()->set_file(project->file());
    event.mutable_project()->set_date(project->date());
    event.mutable_project()->set_total(project->total());
    event.mutable_project()->set_tanksize(project->tanksize());
    event.mutable_project()->mutable_network()->CopyFrom(project->network());
    pushToConsole(comm::kit::ProjectUpdated::kEXTFieldNumber, event);
}

void kit::Manager::pushToConsole(uint16_t type, const google::protobuf::Message &m)
{
    if (connection_ != nullptr) connection_->sendMessage(0, type, m);
}

void kit::Manager::load()
{
    DLOG(INFO) << "Load projects";

    int fd = open("projects.pb", O_RDONLY);
    if (fd == -1) fd = open("build/default/install-root/projects.pb", O_RDONLY);
    google::protobuf::io::FileInputStream fstream(fd);
    if (google::protobuf::TextFormat::Parse(&fstream, &projects_))
    {
        DLOG(INFO)<<"Project size: " << projects_.projects_size();
    }
    else DLOG(INFO)<<"Error in reading data from projects.pb";
    close(fd);
}

void kit::Manager::save()
{
    boost::mutex::scoped_lock lock(mutex_);
    int fd = open("projects.pb", O_WRONLY);
    if (fd == -1) fd = open("build/default/install-root/projects.pb", O_WRONLY);

    google::protobuf::io::FileOutputStream fstream(fd);

    if (google::protobuf::TextFormat::Print(projects_, &fstream))
    {
        DLOG(INFO)<<"Data saved";
        if(project_id_ != "")
        {
            auto file = this->project()->file().c_str();

            DLOG(INFO)<<"Remain ="<<this->project()->remain();
            DLOG(INFO)<<"Samples size ="<<data_.samples_size();

            int fd = open(file, O_WRONLY);
            if (fd == -1)
            {
                DLOG(INFO)<<"Error open file "<<file;
                return;
            }

            google::protobuf::io::FileOutputStream fstream_sample(fd);

            if (google::protobuf::TextFormat::Print(data_, &fstream_sample)) DLOG(INFO)<<"Data sample saved";
            else DLOG(INFO)<<"Error in saving data to "<<file;
        }
    }
    else DLOG(INFO)<<"Error in saving data to data.pb";
    close(fd);
}

void kit::Manager::loadData()
{
     auto file = this->project()->file().c_str();
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        DLOG(INFO)<<"Error open file " << file;
        return;
    }

    google::protobuf::io::FileInputStream fstream(fd);
    if (google::protobuf::TextFormat::Parse(&fstream, &data_))
    {
        DLOG(INFO)<<"Sample size: " << data_.samples_size();
        uint32_t remain = this->project()->total() - static_cast<uint32_t>(this->data().samples_size());
        this->project()->set_remain(remain);
    }
    else DLOG(INFO)<<"Error in reading data from " << file;

    close(fd);
}

void kit::Manager::updateState(State state)
{
    DLOG(INFO)<<"updateState " << state;
    state_ = state;
    comm::kit::StateChanged event;
    event.set_state(state_);
    if (project_id_ != "") event.set_current(project_id_);
    pushToConsole(comm::kit::StateChanged::kEXTFieldNumber, event);
}

void kit::Manager::changeProject(std::string id)
{    
    project_id_ = id;
    tank_->updateSize(project()->tanksize());
    loadData();
}

data::kit::Project* kit::Manager::project()
{
    if(project_id_ != "")
    {
        for (int i = 0; i < projects_.projects_size(); i++)
        {
            if (projects_.projects(i).id() == project_id_) return projects_.mutable_projects(i);
        }
    }
    return nullptr;
}
