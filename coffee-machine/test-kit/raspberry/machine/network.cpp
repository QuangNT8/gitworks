#include "manager.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

void kit::Manager::build(std::string id)
{
    DLOG(INFO) << "Build network";
    if(state_ != STOPPED) DLOG(INFO) << "The system is still running. You cannot build network !";
    else
    {
        updateState(NETWORK_BUILDING);

        for(int i = 0; i < projects_.projects_size(); i++)
        {
            auto project = projects_.mutable_projects(i);
            if(project->id() == id)
            {
                ann::Network* net = nets_[id];
                if(net != nullptr) delete net;
                net = ann::Network::build();
                data::kit::File file;

                int fd = open(project->file().c_str(), O_RDONLY);
                if (fd == -1) fd = open(("build/default/install-root/" + project->file()).c_str(), O_RDONLY);

                google::protobuf::io::FileInputStream fstream(fd);
                if (google::protobuf::TextFormat::Parse(&fstream, &file))
                {
                    //FIXME
                    //max: 2500	500	2000
                    //min: 0     0	0
                    close(fd);
                    file.clear_maxs();
                    file.clear_mins();
                    file.add_maxs(2500);
                    file.add_maxs(500);
                    file.add_maxs(2000);
                    file.add_mins(0);
                    file.add_mins(0);
                    file.add_mins(0);

                    net->loadTrainingData(file);
                    for(int i = 1; i <= 100 && state_ == NETWORK_BUILDING; i++)
                    {
                        net->train(DEFAULT_TRAINING_EPOCHS_PER_SAMPLE/100);
                        DLOG(INFO) << "training " << i << "% > recent average error: " << net->getRAE();
                    }
                    if(state_ == NETWORK_BUILDING)
                    {
                        project->mutable_network()->CopyFrom(net->data());
                        Manager::instance()->buildingCompleted(project);
                    }

                    this->save();
                }
                else DLOG(INFO)<<"Error in reading data from " << project->file();
                nets_[id] = net;
                break;
            }
        }
    }
}


double kit::Manager::predict(double v_out, std::string id)
{
    if(id == "") id = project_id_;
    if(id == "") DLOG(INFO) << "project_id is null!";
    else
    {
        for(int i = 0; i < projects_.projects_size(); i++)
        {
            auto project = projects_.mutable_projects(i);
            if(project->id() == id)
            {
                ann::Network* net = nullptr;
                if(!project->has_network())
                {
                    DLOG(INFO) << "load default network";
                    net = ann::Network::load("model_default.pb");
                    if(net == nullptr) net = ann::Network::load("build/default/install-root/model_default.pb");
                    nets_[id] = net;
                }
                else
                {
                    net = nets_[id];
                    if(net == nullptr)
                    {
                        DLOG(INFO) << "reload network";
                        net = ann::Network::load(project->network());
                        nets_[id] = net;
                    }
                }
                vector<double> input;
                input.push_back(weight_);
                input.push_back(v_out);
                double t_valve = net->predict(input)[0];
                input.clear();
                DLOG(INFO) << "predict " << weight_ << " " << v_out << " --> " << t_valve*20 << "ms";
                return t_valve;
            }
        }
    }
    return 0.0;
}

void kit::Manager::learn(double v_tank, double v_out, uint16_t t_valve)
{
    DLOG(INFO) << "Learning";
    if(state_ != SAMPLING) DLOG(INFO) << "The system is not sampling. You cannot call learn !";
    else
    {
        for(int i = 0; i < projects_.projects_size(); i++)
        {
            auto project = projects_.mutable_projects(i);
            if(project->id() == project_id_)
            {
                ann::Network* net = nullptr;
                if(!project->has_network())
                {
                    net = ann::Network::load("model_default.pb");
                    if(net == nullptr) net = ann::Network::load("build/default/install-root/model_default.pb");
                    net->loadTrainingData(data_);
                }
                else
                {
                    net = nets_[project_id_];
                    if(net == nullptr)
                    {
                        net = ann::Network::load(project->network());
                        net->loadTrainingData(data_);
                    }
                }
                net->pushTrainingData(v_tank, v_out, t_valve);
                net->train(net->trainingDataSize());
                nets_[project_id_] = net;
                DLOG(INFO) << "training " << i << "% > recent average error: " << net->getRAE();
            }
        }
    }
}

