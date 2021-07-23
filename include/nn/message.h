//
// Created by Khurram Javed on 2021-04-01.
//

#ifndef INCLUDE_NN_MESSAGE_H_
#define INCLUDE_NN_MESSAGE_H_


class message {
 public:
    float gradient;
    int time_step;
    int distance_travelled;
    float lambda;
    float gamma;
    float error;
    float error_shadow_prediction;

    message(float m, int t);
};

class message_activation {
 public:
    float gradient_activation;
    float error_prediction_value;
    int time;
    float TH;
};


#endif  // INCLUDE_NN_MESSAGE_H_p