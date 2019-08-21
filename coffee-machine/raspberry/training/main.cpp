#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include "ann/neuronnetwork.h"
#include "glog/logging.h"

template<typename type>
string vector_to_string(const vector<type> vec)
{
    ostringstream oss;

    if (!vec.empty())
    {
        std::copy(vec.begin(), vec.end()-1, ostream_iterator<type>(oss, ","));
        oss << vec.back();
    }

    return oss.str();
}

int main()
{
//    vector<unsigned> layers = {2, 3, 1 };
    vector<unsigned> layers = {2, 3, 5, 3, 1 };


//    ann::Network* net1 = ann::Network::build(layers);
////    ann::Network* net1 = ann::Network::build();
//    net1->loadTrainingData("trainingData2.txt");
//    for(int i = 1; i <= 100; i++)
//    {
//        net1->train(DEFAULT_TRAINING_EPOCHS_PER_SAMPLE/100);
//        DLOG(INFO) << "training " << i << "% > recent average error: " << net1->getRAE();
//    }
////    net1->save("trained_model.txt", ann::Network::TEXT);
//    net1->save("trained_model2.pb");


//    ann::Network* net2 = ann::Network::load("trained_model.txt", ann::Network::TEXT);
    ann::Network* net2 = ann::Network::load("trained_model2.pb");

    vector<double> input;
    input.push_back(2020);
    input.push_back(200);
    net2->predict(input);
    DLOG(INFO) << net2->predict(input)[0];
    //        vector<double> target = testData.output(i);
    //        vector<double> output = net2->predict(input);


//    if(net2 == nullptr)
//        DLOG(INFO) << "loading failed";

//    ann::TrainingData testData("testingData3.txt", 2, 1);

//    double total = 0.0;
//    for(unsigned int i = 0; i < testData.training_size(); i++)
//    {
//        vector<double> input = testData.input(i);
//        vector<double> target = testData.output(i);
//        vector<double> output = net2->predict(input);

//        double error = 0.0;

//        for(unsigned int j = 0; j < output.size(); j++)
//        {
//            error += abs(output[j]-target[j])/target[j];
//        }

//        error /= output.size();
//        total += error;

//        DLOG(INFO) << "testing " << i + 1 << " > input " << vector_to_string(input) << " output: " << vector_to_string(output) << " expected " << vector_to_string(target) << " > error " << 100*error << "%";
//    }
//    DLOG(INFO) << "----------------------------------------------------------";
//    DLOG(INFO) << "overal net error " << 100*total/testData.training_size() << "%";

    delete net2;
//    delete net1;
    DLOG(INFO) << "Done";

    return 0;
}
