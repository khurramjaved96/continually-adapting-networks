#! /bin/sh

./FlexibleNN \
--name hereami \
--max_episodes 2000000 \
--seed 5 \
--steps 40000000 \
--width 10 \
--step_size 3e-5 \
--num_layers 10 \
--data_driven_initialization false \
--add_features false \
--features_min_timesteps 100000 \
--features_acc_thresh 0.90 \
--num_new_features 5 \
--sparsity 0 \
--gamma 0.98  \
--epsilon 0.10 \
--episode_length 1000 \
--episode_gap 5 \
--tmaze_corridor_length 2 \
--prediction_problem false \
--comment $1