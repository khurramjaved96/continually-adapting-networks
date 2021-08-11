//
// Created by Khurram Javed on 2021-08-11.
//

#include "../include/random_data_generator.h"

RandomDataGenerator::RandomDataGenerator() {
  std::vector<float>
      l1{39, -59, 0, -52, -12, -83, -6, 62, 7, -15, -98, -19, 85, -46, 74, 27, 31, -17, 54, 74, 90, -30, -77, 29, 90,
         -84, 25, -50, 81, 18, -81, -60, 76, -97, 58, -43, -73, -1, -100, 37, 0, -15, -62, -27, -38, -86, -38, 82, -58,
         95, 50, 81, 68, 69, 19, -42, 90, -21, 62, -66, -53, -83, 79, -17, -23, -95, -40, -21, -59, 75, -91, 7, 52, 26,
         91, -19, 50, -15, 62, 62, 39, -30, 18, 39, 54, 82, -19, 27, -68, 65, -73, -46, 17, 53, 66, -59, 54, 34, -47,
         -8};

  std::vector<float>
      l2{34, -61, 75, -86, -98, -55, 31, -74, 21, -48, -87, 56, -72, 93, 10, 67, -21, -3, 79, -36, -29, 69, -30, 49,
         84, 97, -40, -78, 51, 57, -14, 23, -51, 96, 23, 42, -55, 72, -42, 22, 54, -45, 72, -24, -58, 15, 99, -84, -77,
         -68, 67, -50, -75, -40, 96, 99, -59, 80, 93, -75, -67, 83, 47, 6, -39, -92, 44, -58, 67, -63, -12, -77, -8,
         59, -56, -66, -28, 14, 35, -88, 43, 25, -8, 78, 44, 31, 76, -80, 94, -46, -49, -75, 55, -36, -12, -91, 79,
         -79, 17, 78};

  std::vector<float>
      l3{-65, -18, 63, 25, 61, 97, -17, 26, 79, -79, -22, 77, 6, 35, -85, 1, 52, 12, -18, 56, 10, 25, 76, 85, 42, -54,
         -13, 34, -64, 16, -26, 42, -52, -45, 80, -88, 34, -57, -49, -70, -15, -81, 16, -18, -82, 3, -71, -31, 39, -68,
         42, 100, 86, -24, 86, -21, 0, 92, -86, 36, -68, 55, -10, -88, -41, -14, -61, -5, 11, -89, 16, -70, 80, 85,
         -70, 72, -4, 87, 83, 6, -75, 69, 65, 71, -72, -73, -21, 61, -68, 61, -55, -41, 39, 14, 34, -3, 71, 11, 15,
         -58};

  std::vector<float> targets_temp
      {-93, 33, 28, -83, -42, -70, 61, -10, -79, -2, -60, -64, 59, -9, -66, 15, 84, 16, -67, -94, 27, -4, 82, -21,
       90, 84, 59, -81, 1, -41, -6, 96, -78, -71, -22, -59, -47, -12, 51, -50, -39, -87, 80, 32, -96, 2, 29, 6, 64,
       18, 48, 80, -45, -53, -65, 16, -16, -36, 54, 25, -41, -69, 91, 7, -60, 46, 13, -10, -67, -20, 56, 68, 2, 99,
       -77, -38, -26, -95, -35, -94, -59, -18, -13, -44, -42, 31, -59, -65, -91, 97, 47, 11, 91, 22, -86, -61, -31,
       97, -99, -97};

  for (int a = 0; a < 110; a++) {
    std::vector<float> curr_inp;
    std::vector<float> curr_target;
    if (a < 100) {
      curr_inp.push_back(l1[a]);
      curr_inp.push_back(l2[a]);
      curr_inp.push_back(l3[a]);
      curr_target.push_back(targets_temp[a]);

    } else {
      curr_inp.push_back(0);
      curr_inp.push_back(0);
      curr_inp.push_back(0);
    }
    inputs.push_back(curr_inp);
    targets.push_back(curr_target);
  }
}

std::vector<float> RandomDataGenerator::get_input() {
  return inputs[this->current_position];
}

std::vector<float> RandomDataGenerator::get_target() {
  return targets[this->current_position];
}

bool RandomDataGenerator::step() {
  this->current_position++;
  if(this->current_position == this->inputs.size())
    return true;
  return false;
}