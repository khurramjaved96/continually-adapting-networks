//
// Created by Khurram Javed on 2021-04-01.
//

#ifndef INCLUDE_NN_NETWORKS_ADAPTIVE_NETWORK_H_
#define INCLUDE_NN_NETWORKS_ADAPTIVE_NETWORK_H_


#include <vector>
#include <map>
#include <random>
#include <string>
#include "../synapse.h"
#include "../neuron.h"
#include "../dynamic_elem.h"
#include "./network.h"


class ContinuallyAdaptingNetwork : public Network {

public:


    ContinuallyAdaptingNetwork(float step_size, int seed, int no_of_input_features);

    ~ContinuallyAdaptingNetwork();

    void print_graph(neuron *root);

    void viz_graph();

    void set_print_bool();

    std::string get_viz_graph();

    float introduce_targets(std::vector<float> targets);

    float introduce_targets(std::vector<float> targets, float gamma, float lambda);

//    void add_memory(float step_size);
    void add_feature(float step_size);
//    std::vector<float> get_memory_weights();
};

#endif  // INCLUDE_NN_NETWORKS_ADAPTIVE_NETWORK_H_