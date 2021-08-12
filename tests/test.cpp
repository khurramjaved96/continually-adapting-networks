//
// Created by Khurram Javed on 2021-07-19.
//


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>

#include "include/catch3/catch_amalgamated.hpp"
#include "include/test_cases.h"

//TEST_CASE("Recurrent gradient", "[Gradient estimation]") {
//  REQUIRE(recurrent_network_test());
//}

TEST_CASE("Feedforward deep gradients with variable length paths", "[Gradient estimation]") {
  REQUIRE(feedforwadtest_relu());
}

TEST_CASE("Feedforward deep gradients with variable length paths Sigmoid Activation", "[Gradient estimation]") {
  REQUIRE(feedforwadtest_sigmoid());
}

TEST_CASE("Feedforward deep gradients with variable length paths LeakyReLU Activation", "[Gradient estimation]") {
  REQUIRE(feedforwardtest_leaky_relu());
}

TEST_CASE("Target value test", "[Gradient estimation]") {
  REQUIRE(forward_pass_without_sideeffects_test());
}

TEST_CASE("Gradient estimation with mixed activations", "[Gradient estimation]") {
  REQUIRE(feedforward_mixed_activations());
}

TEST_CASE("Gradient estimation relu with random inputs", "[Gradient estimation]") {
  REQUIRE(feedforwadtest_relu_random_inputs());
}

TEST_CASE("Lambda Returns", "[Gradient estimation]") {
  REQUIRE(lambda_return_test());
}

TEST_CASE("TIDBD Test", "[Gradient estimation]") {
  REQUIRE(train_single_parameter());
}

TEST_CASE("TIDBD Test MultiLayer", "[Gradient estimation]") {
  REQUIRE(train_single_parameter_two_layers());
}
//}