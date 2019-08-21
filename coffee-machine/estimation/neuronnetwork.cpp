#include "neuronnetwork.h"
#include <random>
#include <chrono>

ann::Network* ann::Network::build(std::vector<int> layers)
{
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);

    if (layers.size()<3) return nullptr;
    auto ret = new Network();

    for (auto c: layers)
    {
        Layer layer;

        if (ret->layers_.size()==0) //input layer
        {
            for (int i=0; i<c; i++){layer.push_back(new Neuron());}
        }
        else
        {
            //double wFactor = 0.01;
            double wFactor = sqrt(2.0/ret->layers_.size());
            //double wFactor = sqrt(2/(ret->layers_.size()+c));

            Layer& prev = ret->layers_[ret->layers_.size()-1];
            for (int i=0; i<c; i++)
            {
                auto n = new Neuron();
                n->bias_ = 0.0;
                for (auto p: prev)
                {
                    auto s = new Synapse(p,n);
                    s->weight_ = unif(rng) * wFactor;
                    p->outputs_.push_back(s);
                    n->inputs_.push_back(s);
                    ret->synapses_.push_back(s);
                }
                layer.push_back(n);
            }
        }
        ret->layers_.push_back(layer);
    }
    return ret;
}

void ann::Network::destroy()
{
    for (auto& l: layers_)
    {
        for (auto n: l)
        {
            delete n;
        }
    }
    for (auto s: synapses_)
    {
        delete s;
    }
    synapses_.clear();
    layers_.clear();
}

#include <iostream>
#include <fstream>

using namespace std;

ann::Network* ann::Network::load(std::string file)
{
    ifstream infile( file );
    if( ! infile.is_open() )
    {
        std::cout << "file " << file << " not found!";
        return nullptr;
    }

    // read the whole file
    std::string netAsBuffer((std::istreambuf_iterator<char>(infile)),
                            std::istreambuf_iterator<char>());

    infile.close();
    //return Network::deserialize( netAsBuffer );
    /*TODO:*/
}

void ann::Network::save(std::string file)
{
    ofstream outfile;
    outfile.open( file );

    if( ! outfile.is_open() )
    {
        std::cout << "cannot open file " << file << " to write";
        return ;
    }

    //    netFile << serialize();
    /*TODO:*/
    outfile.close();
}
