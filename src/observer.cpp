#include "observer.h"
observer::observer(float Ts)   // constructor
{
    this->Ts = Ts;
    init_matrices();
}
observer::~observer() {}

// init the A, B, C, H Matrices
void observer::init_matrices()
{}
// calculate the observed states -> calc sum, integrate, store in x_hat
void observer::do_step(float u,float y_meas)
{}
// to the integration dxdt -> x, use trapezoidal form
void observer::integrate_states()
{}
// get the observed states
Matrix<float,N,1> observer::get_x_obsv()
{
    return x_hat;
}