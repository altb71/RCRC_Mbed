#pragma once

#include <Eigen/Dense>
#include <cstdint>

#include "mbed.h"
#define N 2      // number of states
#define N_meas 1 // number measurements

using namespace Eigen;

class observer
{
public:
    observer() {};                    // default constructor
    observer(float);                  // constructor
    virtual ~observer();              // deconstructor
    void do_step(float, float);       // calculate one step of the observer
    Matrix<float, N, 1> get_x_obsv(); // get the observed states

private:
    Matrix<float, N, N> A;
    Matrix<float, N, 1> B;
    Matrix<float, N_meas, N> C;
    Matrix<float, N, N_meas> H;
    Matrix<float, N, 1> dxdt_hat, x_hat;
    float Ts;
};
