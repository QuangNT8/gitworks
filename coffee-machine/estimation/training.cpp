#include <iostream>
#include "neuronnetwork.h"

void ann::Neuron::calculate()
{
    double sum = bias_;
    for (auto s: inputs_)
    {
        sum += s->weight()* s->left()->value();
    }
    value_ = sigmoid_(sum);
    derivative_ = value_ * (1.0-value_);
}

void ann::Network::forward(std::vector<double> input)
{
    auto ins = this->inputs();
    if (input.size() != ins.size())
    {
        std::cout << "input vector size (" << input.size() << ") does not match network input dimension (" << ins.size() << ")!";
        return;
    }

    for (int i=0; i<ins.size(); i++)
    {
        ins[i]->value_ = input[i];
    }

    for (int i =1; i<layers_.size(); i++)
    {
        for (auto n: layers_[i])
        {
            n->calculate();
        }
    }
}

void ann::Network::backward(std::vector<double> expecteds)
{
    auto outs = outputs();
    if (expecteds.size() != outs.size())
    {
        std::cout << "output vector size (" << expecteds.size() << ") does not match network outs dimension (" << outs.size() << ")!";
        return;
    }

    for (int i=0; i<outs.size(); i++)
    {
        outs[i]->gradient_ = outs[i]->derivative_ * (expecteds[i] - outs[i]->value_);
    }

    for (int i =layers_.size()-1; i >= 0; i--)
    {
        for (auto n: layers_[i])
        {
            double sum = 0;
            for (auto s: n->outputs_)
            {
                sum += s->weight_ * s->right_->gradient_;
            }
            n->gradient_ = n->derivative_ * sum;
        }
    }

    /*update weights*/
    for (int i =0; i < layers_.size()-1; i++)
    {
        for (auto n: layers_[i])
        {
            for (auto s: n->inputs_)
            {
                s->weight_ += wRate_ * s->left_->value_ * s->right_->gradient_;
            }
            n->bias_ += bRate_ * n->gradient_;
        }
    }
}

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void ann::Network::train(std::string file, unsigned int epochs)
{
    std::vector<std::vector<double>> training_set;

    std::ifstream infile;
    infile.open (file);

    while(!infile.eof())
    {
        std::string line;

        getline (infile, line);
        std::stringstream ss(line);
        std::vector<double> drow;
        while(!ss.eof())
        {
            double dcell;
            ss >> dcell;
            drow.push_back(dcell);
            std::cout << dcell << " ";
        }

        training_set.push_back(drow);
        std::cout << "\n";
    }

   infile.close();
   /*TODO:*/
}
