//
// Created by Khurram Javed on 2021-07-11.
//

#ifndef FLEXIBLENN_ADAPTIVE_RECURRENT_NETWORK_H
#define FLEXIBLENN_ADAPTIVE_RECURRENT_NETWORK_H



#include "../synapse.h"
#include <vector>
#include "../neuron.h"
#include <vector>
#include <map>
#include <random>
#include "../dynamic_elem.h"




class ContinuallyAdaptingRecurrentNetwork{



    long long int time_step;

    std::mt19937 mt;


public:
    std::vector<neuron*> output_neurons;
    neuron* error_neuron;
    std::vector<synapse*> all_synapses;
    std::vector<synapse*> output_synapses;
    std::vector<neuron*> error_predicting_neurons;
//  all_heap_elements collects all neurons and synapses for easier garbage collection.
    std::vector<dynamic_elem *> all_heap_elements;
    void collect_garbage();
    std::vector<neuron*> all_neurons;
    std::vector<neuron*> input_neurons;
//    std::vector<neuron*> new_features;
    ContinuallyAdaptingRecurrentNetwork(float step_size, int seed, int no_of_input_features);
    ~ContinuallyAdaptingRecurrentNetwork();
    void print_graph(neuron* root);
    void viz_graph();
    void set_print_bool();
    std::string get_viz_graph();
    long long int get_timestep();

    void set_input_values(std::vector<float> const &input_values);
    void step();
    std::vector<float> read_output_values();
    std::vector<float> read_all_values();
    float introduce_targets(std::vector<float> targets);
    float introduce_targets(std::vector<float> targets, float gamma, float lambda);
    int get_input_size();
    int get_total_synapses();
    int get_total_neurons();
//    void add_memory(float step_size);
    void add_feature(float step_size);
//    std::vector<float> get_memory_weights();
};



class ContinuallyAdaptingRecurrentNetworkTest{



    long long int time_step;

    std::mt19937 mt;


public:
    std::vector<neuron*> output_neurons;
    neuron* error_neuron;
    std::vector<synapse*> all_synapses;
    std::vector<synapse*> output_synapses;
    std::vector<neuron*> error_predicting_neurons;
//  all_heap_elements collects all neurons and synapses for easier garbage collection.
    std::vector<dynamic_elem *> all_heap_elements;
    void collect_garbage();
    std::vector<neuron*> all_neurons;
    std::vector<neuron*> input_neurons;
//    std::vector<neuron*> new_features;
    ContinuallyAdaptingRecurrentNetworkTest(float step_size, int seed, int no_of_input_features);
    ~ContinuallyAdaptingRecurrentNetworkTest();
    void print_graph(neuron* root);
    void viz_graph();
    void set_print_bool();
    std::string get_viz_graph();
    long long int get_timestep();

    void set_input_values(std::vector<float> const &input_values);
    void step();
    std::vector<float> read_output_values();
    std::vector<float> read_all_values();
    float introduce_targets(std::vector<float> targets);
    float introduce_targets(std::vector<float> targets, float gamma, float lambda);
    int get_input_size();
    int get_total_synapses();
    int get_total_neurons();
//    void add_memory(float step_size);
    void add_feature(float step_size);
//    std::vector<float> get_memory_weights();
};
#endif //FLEXIBLENN_ADAPTIVE_RECURRENT_NETWORK_H