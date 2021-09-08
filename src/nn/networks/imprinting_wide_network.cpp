#include "../../../include/nn/networks/imprinting_wide_network.h"
#include <iostream>
#include <assert.h>
#include <cmath>
#include <random>
#include <execution>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include "../../../include/nn/neuron.h"
#include "../../../include/nn/synapse.h"
#include "../../../include/nn/dynamic_elem.h"
#include "../../../include/utils.h"
#include "../../../include/nn/utils.h"


ImprintingWideNetwork::ImprintingWideNetwork(int no_of_input_features,
                                             int no_of_output_neurons,
                                             int width_of_network,
                                             std::vector<std::pair<float,float>> input_ranges,
                                             float bound_replacement_prob,
                                             float bound_max_range,
                                             float step_size,
                                             float meta_step_size,
                                             bool tidbd,
                                             int seed) {


  // TODO increment references not handled
  this->time_step = 0;
  this->mt.seed(seed);
  Neuron::gen = std::mt19937(seed);
  std::uniform_real_distribution<float> dist(0, 1);

  this->bound_replacement_prob = bound_replacement_prob;
  this->bound_max_range = bound_max_range; //was 0.05

  if (input_ranges.size() != no_of_input_features){
    std::cout << "input_ranges shape should be equal to no_of_input_features" << std::endl;
    exit(1);
  }

  for (int neuron_no = 0; neuron_no < no_of_input_features; neuron_no++) {
    if (input_ranges[neuron_no].first >= input_ranges[neuron_no].second){
      std::cout << "input_ranges should be [low,high] rather than [high,low]"<< std::endl;
      exit(1);
    }
  }


  this->bias_unit = new BiasNeuron();
  this->bias_unit->is_mature = true;
  this->all_neurons.push_back(bias_unit);

  for (int neuron_no = 0; neuron_no < no_of_input_features; neuron_no++) {
    auto n = new LinearNeuron(true, false);
    n->is_mature = true;
    n->value_ranges = std::make_pair(input_ranges[neuron_no].first, input_ranges[neuron_no].second);
    this->input_neurons.push_back(n);
    this->all_neurons.push_back(n);
  }

  for (int neuron_no = 0; neuron_no < no_of_output_neurons; neuron_no++) {
    auto n = new LinearNeuron(false, true);
    n->is_mature = true;
    this->output_neurons.push_back(n);
    this->all_neurons.push_back(n);
  }

  for (auto &output : this->output_neurons) {
    synapse *s = new synapse(bias_unit, output, 0, step_size);
    s->disable_utility = true;
    this->all_synapses.push_back(s);
    this->output_synapses.push_back(s);
    s->set_meta_step_size(meta_step_size);
    if (tidbd)
      s->turn_on_idbd();
  }


//  Connect our input and output neurons with synapses.
//  for (auto &input : this->input_neurons) {
//    for (auto &output : this->output_neurons) {
//      synapse *s = new synapse(input, output, 0, step_size);
//      this->all_heap_elements.push_back(static_cast<dynamic_elem *>(s));
//      s->increment_reference();
//      this->all_synapses.push_back(s);
//      s->increment_reference();
//      this->output_synapses.push_back(s);
//      s->turn_on_idbd();
//      s->set_meta_step_size(meta_step_size);
//    }
//  }

  for (int neuron_no = 0; neuron_no < width_of_network; neuron_no++) {
    auto n = new BoundedNeuron(false, false, this->bound_replacement_prob, this->bound_max_range);
    this->all_neurons.push_back(n);
    for (int inp_neuron = 0; inp_neuron < no_of_input_features; inp_neuron++) {
      auto s = new synapse(this->input_neurons[inp_neuron], n, 1, 0);
      s->disable_utility = true; // dont want to mark_useless or propagate utility to these
      this->all_synapses.push_back(s);
      n->update_activation_bounds(s);
      s->set_meta_step_size(0);
    }
    for (int out_neuron = 0; out_neuron < no_of_output_neurons; out_neuron++) {
      auto s = new synapse(n, this->output_neurons[out_neuron], 0.0001 * dist(this->mt) , step_size);
      this->output_synapses.push_back(s);
      this->all_synapses.push_back(s);
      s->set_meta_step_size(meta_step_size);
      s->block_gradients();
      if (tidbd)
        s->turn_on_idbd();
    }
  }
}

std::vector<std::vector<std::pair<float, float>>> ImprintingWideNetwork::get_feature_bounds(){
  std::vector<std::vector<std::pair<float,float>>> all_bounds;
  for (auto neuron_it : this->all_neurons){
    if (BoundedNeuron *n = dynamic_cast<BoundedNeuron*>(neuron_it)){
      std::vector<std::pair<float,float>> neuron_bounds;
      for (auto syn_it : n->incoming_synapses)
        neuron_bounds.push_back(n->activation_bounds[syn_it->id]);
      all_bounds.push_back(neuron_bounds);
    }
  }
  return all_bounds;
}

std::vector<float> ImprintingWideNetwork::get_feature_utilities(){
  std::vector<float> feature_utilities;
  for (auto neuron_it : this->all_neurons)
    if (BoundedNeuron *n = dynamic_cast<BoundedNeuron*>(neuron_it))
      feature_utilities.push_back(n->neuron_utility);
  return feature_utilities;
}


void ImprintingWideNetwork::step() {
  //  Calculate and temporarily hold our next neuron values.
  std::for_each(
      std::execution::par_unseq,
      all_neurons.begin(),
      all_neurons.end(),
      [&](Neuron *n) {
        if (BoundedNeuron *ptr = dynamic_cast<BoundedNeuron*>(n))
          ptr->update_value(this->time_step);
        else
          n->update_value(this->time_step);
      });

  std::for_each(
      std::execution::par_unseq,
      all_neurons.begin(),
      all_neurons.end(),
      [&](Neuron *n) {
        n->fire(this->time_step);
      });

//  Contrary to the name, this function passes gradients BACK to the incoming synapses
//  of each neuron.
  std::for_each(
      std::execution::par_unseq,
      all_neurons.begin(),
      all_neurons.end(),
      [&](Neuron *n) {
        n->forward_gradients();
      });

//  Now we propagate our error backwards one step
  std::for_each(
      std::execution::par_unseq,
      all_neurons.begin(),
      all_neurons.end(),
      [&](Neuron *n) {
        n->propagate_error();
      });

//  Calculate our credit
  std::for_each(
      std::execution::par_unseq,
      all_synapses.begin(),
      all_synapses.end(),
      [&](synapse *s) {
        s->assign_credit();
      });

//  Update our weights (based on either normal update or IDBD update
  std::for_each(
      std::execution::par_unseq,
      all_synapses.begin(),
      all_synapses.end(),
      [&](synapse *s) {
        s->update_weight();
      });

//  Mark all is_useless weights and neurons for deletion
  std::for_each(
      std::execution::par_unseq,
      all_neurons.begin(),
      all_neurons.end(),
      [&](Neuron *n) {
        n->mark_useless_weights();
      });

//  Delete our is_useless weights and neurons
  std::for_each(
      all_neurons.begin(),
      all_neurons.end(),
      [&](Neuron *n) {
        n->prune_useless_weights();
      });

//  For all synapses, if the synapse is is_useless set it has 0 references. We remove it.

  std::for_each(
      std::execution::par_unseq,
      this->all_synapses.begin(),
      this->all_synapses.end(),
      [&](synapse *s) {
        if (s->is_useless) {
          s->decrement_reference();
        }
      });
  auto it = std::remove_if(this->all_synapses.begin(), this->all_synapses.end(), to_delete_s);
  this->all_synapses.erase(it, this->all_synapses.end());

//  Similarly for all outgoing synapses and neurons.
  std::for_each(
      std::execution::par_unseq,
      this->output_synapses.begin(),
      this->output_synapses.end(),
      [&](synapse *s) {
        if (s->is_useless) {
          s->decrement_reference();
        }
      });
  it = std::remove_if(this->output_synapses.begin(), this->output_synapses.end(), to_delete_s);
  this->output_synapses.erase(it, this->output_synapses.end());

  std::for_each(
      std::execution::par_unseq,
      this->all_neurons.begin(),
      this->all_neurons.end(),
      [&](Neuron *s) {
        if (s->useless_neuron) {
          s->decrement_reference();
        }
      });

  auto it_n = std::remove_if(this->all_neurons.begin(), this->all_neurons.end(), to_delete_n);
  this->all_neurons.erase(it_n, this->all_neurons.end());

  this->time_step++;
}
