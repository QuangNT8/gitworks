#ifndef NEURON_NETWORK_H
#define NEURON_NETWORK_H
#include <string>
#include <cmath>
#include <vector>
#include "../commons/data.bj.pb.h"
#include "../commons/data.kit.pb.h"

/*
 * 1. To train a neureal network:
 *
 *  ann::Network* net = ann::Network::build({2, 3, 5, 3, 1 });
 *  net->loadTrainingData("trainingData.txt");
 *  net->train(10000);
 *  net->saveTrainedModel("trained_model.txt");
 *
 * 2. To load a neureal network from trained model file
 *
 *  ann::Network* net = ann::Network::loadTrainedModel("trained_model.txt");
 *
 * 3. To build a neureal network from vector<unsigned> layers and vector<double> weights:
 *
 *  ann::Network* net = ann::Network::build(layers);
 *  net->setVectorMax({2000, 200, 1000});
 *  net->setVectorMin({0, 0, 50});
 *  net->setWeights(weights);
 *
 * 4. To predict the output of a vector<double> input:
 *
 *  vector<double> output = net->predict(input);
 */

#define DEFAULT_TRAINING_EPOCHS_PER_SAMPLE 100000

using namespace std;

namespace ann
{
class Neuron;
class Network;
typedef vector<Neuron*> Layer;

class TrainingData
{
public:
    TrainingData(const string file, unsigned input_size, unsigned output_size, bool vector_minmax = false);
    TrainingData(const data::kit::File file, unsigned input_size, unsigned output_size, bool vector_minmax = false);
    void destroy();
    ~TrainingData(){this->destroy();}
    unsigned training_size() {return static_cast<unsigned>(inputs_.size());}
    void push(double vtank, double vout, double time);
    vector<double> input(unsigned i) {return inputs_[i];}
    vector<double> output(unsigned i) {return outputs_[i];}
    vector<double> vector_min() {return vector_min_;}
    vector<double> vector_max() {return vector_max_;}
private:
    void normalize();
    unsigned input_size_;
    unsigned output_size_;
    vector<vector<double>> inputs_;
    vector<vector<double>> outputs_;
    vector<double> vector_min_;
    vector<double> vector_max_;
};

struct Connection
{
    double weight;
    double delta_weight;
    Connection(){weight = rand() / double(RAND_MAX);}
};

class Neuron
{
public:
    double value(){return value_;}
    void calculate();
private:
    Neuron(unsigned outputs, unsigned index);

    static inline double transfer(double x){return tanh(x);}
    static inline double dtransfer(double x){return 1.0 - x * x;}

    double bias_ = 0.0;
    double value_ = 0.0;
    double gradient_ = 0.0;
    double delta_bias_ = 0.0;
    vector<Neuron*> inputs_;
    vector<Connection> outputs_;
    unsigned index_;

    friend Network;
};

class Network
{
public:
    void destroy();
    ~Network(){this->destroy();}

    static Network* build();
    static Network* build(const vector<unsigned> layers);
    static Network* load(data::bj::Network data);
    static Network* load(string file);
    data::bj::Network data();
    bool loadTrainingData(string file);
    bool loadTrainingData(data::kit::File file);
    void pushTrainingData(double vtank, double vout, double time);
    void train(unsigned epochs_per_sample = 0);
    void save(string file);

    vector <unsigned> getTopology();
    void setLearningRate(double eta, double alpha){eta_ = eta; alpha_ = alpha;}
    void setVectorMax(const vector<double> &vector_max);
    void setVectorMin(const vector<double> &vector_min);

    vector<double> predict(const vector<double> &input);

    Layer& inputs(){return layers_[0];}
    Layer& outputs(){return layers_[layers_.size()-1];}

    double getRAE() const { return recent_average_error_; }

    uint32_t trainingDataSize() {if(training_data_ != nullptr) training_data_->training_size(); }
private:
    Network(){}

    void forward(const vector<double> &input);
    void backward(const vector<double> &target);

    vector<double> vector_max_;
    vector<double> vector_min_;
    vector<Layer> layers_; //layers_[layer][neuron]
    double eta_ = 0.15; // learning rate
    double alpha_ = 0.5; // momentum

    double error_;
    double recent_average_error_;
    double recent_average_smoothing_factor_ = 100.0;

    ann::TrainingData* training_data_ = nullptr;
};

}

#endif // NEURON_NETWORK_H
