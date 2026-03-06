#include "observer.h"

// constructor
observer::observer(float Ts)
{
    this->Ts = Ts;

    A.setZero();
    B.setZero();
    C.setZero();
    H.setZero();
    dxdt_hat.setZero();
    x_hat.setZero();
}

observer::~observer() {}

// calculate one step of the observer
void observer::do_step(float u, float y_meas)
{
    // dxdt_hat = ...;
    // x_hat += ...;
}

// get the observed states
Matrix<float, N, 1> observer::get_x_obsv() { return x_hat; }
