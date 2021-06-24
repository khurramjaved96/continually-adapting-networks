//
// Created by taodav on 23/6/21.
//

#include "../../../include/neural_networks/networks/test_skip.h"
#include "../../../include/neural_networks/neuron.h"

TestSkip::TestSkip(float step_size, int width, int seed) : TestCase() {

    this->time_step = 0;

    auto in = new neuron(false, false, true);
    this->input_neurons.push_back(in);
    this->all_neurons.push_back(in);


    bool relu = true;
    auto n = new neuron(relu, false);
    this->all_neurons.push_back(n);

    n = new neuron(relu, false);
    this->all_neurons.push_back(n);

    int output_neurons = 1;
    for (int counter=0; counter < output_neurons; counter++)
    {
        auto n = new neuron(false, true);
        this->output_neurons.push_back(n);
        this->all_neurons.push_back(n);
    }


    this->all_synapses.push_back(new synapse(all_neurons[1 - 1], all_neurons[2 - 1], 0.2, step_size));
    this->all_synapses.push_back(new synapse(all_neurons[2 - 1], all_neurons[3 - 1], 0.1, step_size));
    this->all_synapses.push_back(new synapse(all_neurons[2 - 1], all_neurons[4 - 1], 0.4, step_size));
    this->all_synapses.push_back(new synapse(all_neurons[3 - 1], all_neurons[4 - 1], 0.3, step_size));

    for(auto it: this->all_synapses){
        sum_of_gradients.push_back(0);
    }

}