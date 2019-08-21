#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
//#include "glog/logging.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <sys/fcntl.h>
#include "neuronnetwork.h"


ann::TrainingData::TrainingData(const string file, unsigned input_size, unsigned output_size, bool vector_minmax)
{
    input_size_ = input_size;
    output_size_ = output_size;
    ifstream infile;
    infile.open(file.c_str());
    // vector_minmax
    if(vector_minmax)
    {
        if(!infile.eof())
        {
            string line;
            getline(infile, line);
            stringstream ss(line);
            for(unsigned i = 0; i < input_size + output_size; i++)
            {
                double value;
                ss >> value;
                if(ss.fail())
                {
                    vector_max_.clear();
                    return;
                }
                vector_max_.push_back(value);
            }
        }
        if(!infile.eof())
        {
            string line;
            getline(infile, line);
            stringstream ss(line);
            for(unsigned i = 0; i < input_size + output_size; i++)
            {
                double value;
                ss >> value;
                if(ss.fail())
                {
                    vector_max_.clear();
                    vector_min_.clear();
                    return;
                }
                vector_min_.push_back(value);
            }
        }
    }
    // data
    while(!infile.eof())
    {
        string line;
        getline(infile, line);
        if(line.size() > 0)
        {
            vector<double> input;
            vector<double> output;
            stringstream ss(line);
            for(unsigned int i = 0; i < input_size; i++)
            {
                double value;
                ss >> value;
                if(ss.fail())
                {
                    inputs_.clear();
                    outputs_.clear();
                    return;
                }
                input.push_back(value);
            }
            for(unsigned int i = 0; i < output_size; i++)
            {
                double value;
                ss >> value;
                if(ss.fail())
                {
                    inputs_.clear();
                    outputs_.clear();
                    return;
                }
                output.push_back(value);
            }
            inputs_.push_back(input);
            outputs_.push_back(output);
        }
    }
    if(vector_minmax) normalize();
    infile.close();
}

ann::TrainingData::TrainingData(const data::kit::File file, unsigned input_size, unsigned output_size, bool vector_minmax)
{
    input_size_ = input_size;
    output_size_ = output_size;
    // vector_minmax
    if(vector_minmax)
    {
        for(auto d: file.mins()) vector_min_.push_back(d);
        for(auto d: file.maxs()) vector_max_.push_back(d);
    }
    // data
    for(auto &sample:file.samples())
    {
        vector<double> input;
        vector<double> output;
        input.push_back(sample.vtank());
        input.push_back(sample.vout());
        output.push_back(sample.time());
        inputs_.push_back(input);
        outputs_.push_back(output);
    }
    if(vector_minmax) normalize();
}

void ann::TrainingData::normalize()
{
    if(vector_max_.size() == input_size_ + output_size_)
    {
        for(unsigned long i = 0; i < inputs_.size(); i++)
        {
            for(unsigned int j = 0; j < input_size_; j++)
                if(vector_max_[j] -  vector_min_[j]> 0)
                    inputs_[i][j] = 1.0*(inputs_[i][j]-vector_min_[j])/(vector_max_[j]-vector_min_[j]);
            for(unsigned int j = 0; j < output_size_; j++)
                if(vector_max_[input_size_ + j] -  vector_min_[input_size_ + j] > 0)
                    outputs_[i][j] = 1.0*(outputs_[i][j]-vector_min_[input_size_ + j])/(vector_max_[input_size_ + j]-vector_min_[input_size_ + j]);
        }
    }
}

void ann::TrainingData::destroy()
{
    inputs_.clear();
    outputs_.clear();
    vector_min_.clear();
    vector_max_.clear();
}

void ann::TrainingData::push(double vtank, double vout, double time)
{
    vector<double> input;
    vector<double> output;
    input.push_back(vtank);
    input.push_back(vout);
    output.push_back(time);
    inputs_.push_back(input);
    outputs_.push_back(output);

}

void ann::Neuron::calculate()
{
    double sum = 0.0;

    // sum the previous layer's outputs (which are our inputs)
    // include the bias node from the previous layer.
    for(auto n: inputs_)
    {
        sum += n->value() * n->outputs_[index_].weight;
    }

    value_ = Neuron::transfer(sum + bias_);
}

ann::Neuron::Neuron(unsigned output_size, unsigned index)
{
    for(unsigned c = 0; c < output_size; c++){
        outputs_.push_back(Connection());
    }

    index_ = index;
}

void ann::Network::forward(const vector<double> &input)
{
    Layer &ins = inputs();

    // check the num of input equal to neuron number expect bias
    if(input.size() != ins.size()) return;

    // assign the input ingredients into the input neurons
    for(unsigned i = 0; i < input.size(); ++i)
        ins[i]->value_ = input[i];

    // forward propagate
    for(unsigned layer = 1; layer < layers_.size(); layer++)
    {
        for(unsigned n = 0; n < layers_[layer].size(); n++)
        {
            layers_[layer][n]->calculate();
        }
    }
}

void ann::Network::backward(const vector<double> &target)
{
    Layer &outs = outputs();

    // calculate overal net error (RMS of output neuron errors)
    error_ = 0.0;

    for(unsigned n = 0; n < outs.size(); ++n)
    {
        double delta = target[n] - outs[n]->value();
        error_ += delta *delta;
    }
    error_ /= outs.size(); // get average error squared
    error_ = sqrt(error_); // RMS

    // implement a recent average measurement:
    recent_average_error_ =
        (recent_average_error_ * recent_average_smoothing_factor_ + error_)
        / (recent_average_smoothing_factor_ + 1.0);

    // calculate output layer gradients
    for(unsigned n = 0; n < outs.size(); ++n)
    {
        double delta = target[n] - outs[n]->value();
        outs[n]->gradient_ = delta * Neuron::dtransfer(outs[n]->value());
    }

    // calculate gradients on hidden layers
    for(unsigned  long l = layers_.size() - 2; l > 0; l--)
    {
        Layer &hidden_layer = layers_[l];
        Layer &next_layer = layers_[l + 1];

        for(unsigned n = 0; n < hidden_layer.size(); n++)
        {
            double sum = 0.0;

            for (unsigned i = 0; i < next_layer.size(); i++)
            {
                sum += hidden_layer[n]->outputs_[i].weight * next_layer[i]->gradient_;
            }

            hidden_layer[n]->gradient_ = sum * Neuron::dtransfer(hidden_layer[n]->value_);
        }
    }

    // update connection weights
    for(unsigned long l = layers_.size() - 1; l > 0; l--)
    {
        Layer &layer = layers_[l];
        Layer &prev_layer = layers_[l - 1];

        for(unsigned n = 0; n < layer.size(); n++)
        {
            for(unsigned i = 0; i < prev_layer.size(); i++)
            {
                Neuron* neuron = prev_layer[i];
                double old_delta_weight = neuron->outputs_[layer[n]->index_].delta_weight;

                double new_delta_weight =
                    // individual input, magnified by the gradient and train rate:
                    eta_ * neuron->value() * layer[n]->gradient_
                    // also add momentum = a fraction of the previous delta weight
                    + alpha_ * old_delta_weight;
                neuron->outputs_[layer[n]->index_].delta_weight = new_delta_weight;
                neuron->outputs_[layer[n]->index_].weight += new_delta_weight;
            }
            double old_delta_bias = layer[n]->delta_bias_;
            double new_delta_bias = eta_ * layer[n]->gradient_ + alpha_ * old_delta_bias;
            layer[n]->delta_bias_ = new_delta_bias;
            layer[n]->bias_ += new_delta_bias;
        }
    }
}

vector<double> ann::Network::predict(const vector<double> &input)
{
    vector<double> result;

    // convert a standard input to normalized input ( every value in [0.0,1.0) )
    vector<double> normalized_input;
    for(unsigned int i = 0; i < input.size(); i++)
        normalized_input.push_back(1.0*(input[i]-vector_min_[i])/(vector_max_[i]-vector_min_[i]));

    forward(normalized_input);

    // convert a normalized output to standard output
    for(unsigned int i = 0; i < layers_.back().size(); i++)
        result.push_back(layers_.back()[i]->value() * (vector_max_[input.size() + i]-vector_min_[input.size() + i]) + vector_min_[input.size() + i]);

    return result;
}

ann::Network* ann::Network::build()
{
    vector<unsigned> layers = {2, 3, 5, 3, 1 };
    return ann::Network::build(layers);
}

ann::Network* ann::Network::build(const vector<unsigned> layers)
{
    if (layers.size()<3) return nullptr;

    auto net = new Network();

    for(unsigned long layer = 0; layer < layers.size(); layer++)
    {
        net->layers_.push_back(Layer());
        auto output_size = layer == layers.size() - 1 ? 0 :layers[layer + 1];

        for(unsigned i = 0; i < layers[layer]; i++){
            Neuron* neuron = new Neuron(output_size, i);
            net->layers_.back().push_back(neuron);
            if(layer > 0 && i < layers[layer])
            {
                for(auto n:net->layers_[layer-1])
                {
                    neuron->inputs_.push_back(n);
                }
            }
        }
    }
    return net;
}

void ann::Network::destroy()
{
    for (auto& l: layers_)
        for (auto n: l) delete n;

    if(training_data_ != nullptr)
        delete training_data_;

    layers_.clear();
    vector_max_.clear();
    vector_min_.clear();
}

bool ann::Network::loadTrainingData(string file)
{
    if(training_data_ != nullptr) training_data_->destroy();

    if(layers_.size() < 3) return false;

    vector<unsigned> topology = getTopology();
    training_data_ = new ann::TrainingData(file, topology[0], topology[topology.size() - 1], true);

    if(training_data_->training_size() > 0)
    {
        setVectorMax(training_data_->vector_max());
        setVectorMin(training_data_->vector_min());
        return true;
    }
    else return false;
}

bool ann::Network::loadTrainingData(data::kit::File file)
{
    if(training_data_ != nullptr) training_data_->destroy();

    if(layers_.size() < 3) return false;

    vector<unsigned> topology = getTopology();

    training_data_ = new ann::TrainingData(file, topology[0], topology[topology.size() - 1], true);

    if(training_data_->training_size() > 0)
    {
        setVectorMax(training_data_->vector_max());
        setVectorMin(training_data_->vector_min());
        return true;
    }
    else return false;
}

void ann::Network::pushTrainingData(double vtank, double vout, double time)
{
    if(training_data_ != nullptr) training_data_->push(vtank, vout, time);
}

void ann::Network::train(unsigned epochs_per_sample)
{
    unsigned epochs;
    if(training_data_ == nullptr || layers_.size() < 3)
        return;

    if(epochs_per_sample == 0)
        epochs = training_data_->training_size() * DEFAULT_TRAINING_EPOCHS_PER_SAMPLE;
    else
        epochs = training_data_->training_size() * epochs_per_sample;

    for(unsigned int i = 0; i < epochs; i++)
    {
        unsigned s = static_cast<unsigned>(rand())%training_data_->training_size();
        vector<double> input = training_data_->input(s);
        vector<double> output = training_data_->output(s);

        forward(input);
        backward(output);
    }
}

ann::Network* ann::Network::load(string file)
{
    Network* net = nullptr;

    data::bj::Network data;
    int fd = open(file.c_str(), O_RDONLY);
    google::protobuf::io::FileInputStream fstream(fd);
    if (google::protobuf::TextFormat::Parse(&fstream, &data))
        return load(data);

    return net;
}

ann::Network* ann::Network::load(data::bj::Network data)
{
    Network* net = nullptr;
    // layers
    vector<unsigned> layers;
    for(auto &layer: data.layers())
        layers.push_back(layer.neurons_size());
    if(layers.size() >= 3) net = Network::build(layers); else return nullptr;
    // vector min max
    for(auto i = 0; i < data.layers(0).neurons_size(); i ++)
    {
        net->vector_min_.push_back(data.layers(0).neurons(i).min());
        net->vector_max_.push_back(data.layers(0).neurons(i).max());
    }
    for(auto i = 0; i < data.layers(data.layers_size() - 1).neurons_size(); i ++)
    {
        net->vector_min_.push_back(data.layers(data.layers_size() - 1).neurons(i).min());
        net->vector_max_.push_back(data.layers(data.layers_size() - 1).neurons(i).max());
    }
    // weights
    for(auto layer = 1 ; layer < net->layers_.size(); layer++)
    {
        for(auto i = 0 ; i < net->layers_[layer].size(); i++)
        {
            int pos = 0;
            net->layers_[layer][i]->bias_ = data.layers(layer).neurons(i).bias();
            for(auto n: net->layers_[layer][i]->inputs_)
            {
                    n->outputs_[net->layers_[layer][i]->index_].weight = data.layers(layer).neurons(i).weights(pos++);
            }
        }
    }

    return net;
}

data::bj::Network ann::Network::data()
{
    data::bj::Network data;
    auto input = data.add_layers();
    //inputs
    for(auto i = 0; i < layers_[0].size(); i++)
    {
        auto neuron = input->add_neurons();
        neuron->set_min(vector_min_[i]);
        neuron->set_max(vector_max_[i]);
    }
    //hiddens
    for(auto i = 1; i < layers_.size() - 1; i++)
    {
        auto layer = data.add_layers();
        for(auto &n: layers_[i])
        {
            auto neuron = layer->add_neurons();
            neuron->set_bias(1.0*static_cast<long>(n->bias_ * 1000000.0)/1000000.0);
            for(auto &i: n->inputs_)
            {
                neuron->add_weights(1.0*static_cast<long>(i->outputs_[n->index_].weight * 1000000.0)/1000000.0);
            }
        }
    }
    //outputs
    auto output = data.add_layers();
    for(auto i = 0; i < layers_[layers_.size() - 1].size(); i++)
    {
        auto neuron = output->add_neurons();
        neuron->set_bias(1.0*static_cast<long>(layers_[layers_.size() - 1][i]->bias_ * 1000000.0)/1000000.0);
        neuron->set_min(vector_min_[layers_[0].size() + i]);
        neuron->set_max(vector_max_[layers_[0].size() + i]);
        for(auto &n: layers_[layers_.size() - 1][i]->inputs_)
        {
            neuron->add_weights(1.0*static_cast<long>(n->outputs_[layers_[layers_.size() - 1][i]->index_].weight * 1000000.0)/1000000.0);
        }
    }
    return data;
}

void ann::Network::save(string file)
{
    int fd = open(file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
    google::protobuf::io::FileOutputStream fstream(fd);
    google::protobuf::TextFormat::Print(data(), &fstream);
}

void ann::Network::setVectorMax(const vector<double> &vector_max)
{
    vector_max_.clear();
    for (unsigned long i=0; i<vector_max.size(); i++)
        vector_max_.push_back(vector_max[i]);
}

void ann::Network::setVectorMin(const vector<double> &vector_min)
{
    vector_min_.clear();
    for (unsigned long i=0; i<vector_min.size(); i++)
        vector_min_.push_back(vector_min[i]);
}

vector<unsigned> ann::Network::getTopology()
{
    vector<unsigned> topology;
    for(auto &layer: layers_)
    {
        topology.push_back(static_cast<unsigned>(layer.size()));
    }

    return topology;
}
