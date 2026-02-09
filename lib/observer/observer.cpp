#include "observer.h"

observer::observer(float Ts) // constructor
{
    this->Ts = Ts;
    init_matrices();
}

observer::~observer() {}

// init the A, B, C, H Matrices
void observer::init_matrices()
{
    // Aufgabe 2.2
    A << -905.387f, 452.694f, 452.694f, -452.694f; // see Matlab
    B << 452.694f, 0.0f;
    H << 30025.468f, 4641.919f; // these values based on poles=1000*(-1+-1j)
    C << 0.0f, 1.0f;
    x_hat.setZero();
    dxdt_old.setZero();
    dxdt.setZero();
}

// calculate the observed states -> calc sum, integrate, store in x_hat
void observer::do_step(float u, float y_meas)
{
    // Aufgabe 2.3, 2.4
    dxdt = B * u + H * (y_meas - C * x_hat) + A * x_hat; // see block-diagram
    integrate_states();
}

// to the integration dxdt -> x, use trapezoidal form
void observer::integrate_states()
{
    // Aufgabe 2.4
    // x_hat += Ts / 2.0f * (dxdt + dxdt_old);
    x_hat += Ts * dxdt;
    dxdt_old = dxdt;
}

// get the observed states
Matrix<float, N, 1> observer::get_x_obsv() { return x_hat; }
