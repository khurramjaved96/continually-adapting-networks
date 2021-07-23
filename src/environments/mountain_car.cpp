//
// Created by taodav on 29/6/21.
// Partially taken from http://incompleteideas.net/MountainCar/MountainCar2.cp
//

#include "../../include/environments/mountain_car.h"
#include <cmath>
#include <random>

/**
 * Mountain Car implementation.
 * States are described by a 2-dimensional vector.
 * The first dimension is position
 * The second is velocity
 * @param seed: Random seed
 * @param tile_coding (CURRENTLY NOT USED)
 */
MountainCar::MountainCar(int seed, int discretization): mt(seed) {
    this->max_position = 0.6;
    this->min_position = -1.2;
    this->max_velocity = 0.07;
    this->min_velocity = -0.07;
    this->goal_position = 0.5;
    this->max_timesteps = 200;

    this->discretization = discretization;

    this->action_sampler = std::uniform_int_distribution<int>(0,3);
    this->current_obs = this->reset();
}

unsigned long MountainCar::observation_shape() {
    Observation obs = get_current_obs();
    if (this->discretization > 0){
        return obs.observation.size();
    }
    return obs.state.size();
}

int MountainCar::n_actions() {
    return 3;
}

bool MountainCar::at_goal() {
    return this->current_obs.state[0] >= this->goal_position;
}

Observation MountainCar::get_current_obs() {
    if (this->discretization > 0) {
        std::vector<float> obs;
        float total_position_range = std::abs(this->max_position - this->min_position);
        float total_velocity_range = std::abs(this->max_velocity - this->min_velocity);

        float biased_position = std::abs(this->current_obs.state[0] - this->min_position);
        float biased_velocity = std::abs(this->current_obs.state[1] - this->min_velocity);

        int position_idx = static_cast <int> (floor((biased_position / total_position_range) * this->discretization));
        int velocity_idx = static_cast <int> (floor((biased_velocity / total_velocity_range) * this->discretization));

//      Set position
        for (int i = 0; i < this->discretization; i++) {
            if (i == position_idx) {
                obs.push_back(1.0);
            } else {
                obs.push_back(0.0);
            }
        }

        for (int i = 0; i < this->discretization; i++) {
            if (i == velocity_idx) {
                obs.push_back(1.0);
            } else {
                obs.push_back(0.0);
            }
        }
        this->current_obs.observation = obs;
    }
    return this->current_obs;

}

int MountainCar::get_random_action() {
    std::vector<float> random_action(3, 0.0);
    int action_idx = action_sampler(mt);
    return action_idx;
}

Observation MountainCar::reset() {
    Observation obs;
    obs.timestep = 0;

    obs.reward = 0.0;
    obs.cmltv_reward = 0;
    obs.is_terminal = false;
    std::vector<float> state{ -0.5, 0.0 };
    obs.state = state;
    this->current_obs = obs;
    return get_current_obs();
}

Observation MountainCar::step(int action) {
    Observation obs;
    obs.state = this->current_obs.state;
    obs.state[1] += (action - 1) * 0.001 + cos(3 * obs.state[0]) * (-0.0025);


    if (obs.state[1] > this->max_velocity) obs.state[1] = this->max_velocity;
    if (obs.state[1] < this->min_velocity) obs.state[1] = this->min_velocity;
    obs.state[0] += obs.state[1];

    if (obs.state[0] > this->max_position) obs.state[0] = this->max_position;
    if (obs.state[0] < this->min_position) obs.state[0] = this->min_position;
    if (obs.state[0] == this->min_position && obs.state[1] < 0) obs.state[1] = 0;

    this->current_obs = obs;
    this->current_obs.is_terminal = this->at_goal();
    this->current_obs.reward = this->at_goal() ? 0.0 : -1.0;
    return get_current_obs();
}