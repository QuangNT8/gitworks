#ifndef NEURONNETWORK_H
#define NEURONNETWORK_H
#include <vector>
#include <string>
#include <cmath>

namespace ann
{

class Synapse;
class Network;
class Neuron;
typedef std::vector<Neuron*> Layer;

class Neuron
{
public:
    double value(){return value_;}
    void calculate();
private:
    Neuron(){}
    inline double sigmoid_(double x) {return 1.0 / (1.0 + exp(-x));}

    double bias_ = 0.0;
    double value_ = 0.0;
    double derivative_ = 0.0;
    double gradient_ = 0.0;
    std::vector<Synapse*> inputs_;
    std::vector<Synapse*> outputs_;
    friend Network;
};

class Synapse
{
public:
    double weight(){return weight_;}
    Neuron* left() {return left_;}
    Neuron* right() {return right_;}
private:
    Synapse(Neuron* left, Neuron* right): left_(left), right_(right){}
    double weight_ = 0.0;
    Neuron* left_ = nullptr;
    Neuron* right_ = nullptr;
    friend Network;
};

class Network
{
public:
    ~Network(){this->destroy();}

    static Network* build(std::vector<int> layers);
    static Network* load(std::string file);
    void save(std::string file);

    void train(std::string file, unsigned int epochs);

    void forward(std::vector<double> input);
    void backward(std::vector<double> expected);

    Layer& inputs(){return layers_[0];}
    Layer& outputs(){return layers_[layers_.size()-1];}

    void destroy();

    void setLearningRate(double w, double b){wRate_ = w; bRate_ = b;}
private:
    Network(){}
    std::vector<Layer> layers_;
    std::vector<Synapse*> synapses_;
    double wRate_; //learning rate of weight
    double bRate_; //learning rate of bias
};

}

#endif // NEURONNETWORK_H
