#include "observer.h"
observer::observer(float Ts)   // constructor
{
    this->Ts = Ts;
    init_matrices();
}
observer::~observer() {}

// init the A, B, C, H Matrices
void observer::init_matrices()
{
    // Aufgabe 2.2
    A << -905.387, 452.694,452.694, -452.694;   // see Matlab
    B << 452.694, 0.000;
    H << 30025.468, 4641.919; // these values based on poles=1000*(-1+-1j)
    C << 0, 1;
    x_hat.setZero();
    dxdt_old.setZero();
}
// calculate the observed states -> calc sum, integrate, store in x_hat
void observer::do_step(float u,float y_meas)
{
    // Aufgabe 2.3, 2.4
    dxdt = B*u + H*(y_meas - C*x_hat) + A*x_hat;    // see block-diagram
    integrate_states();

}
// to the integration dxdt -> x, use trapezoidal form
void observer::integrate_states()
{
    // Aufgabe 2.4
    x_hat += Ts/2*(dxdt + dxdt_old);
    dxdt_old = dxdt;
}
// get the observed states
Matrix<float,N,1> observer::get_x_obsv()
{
    return x_hat;
}