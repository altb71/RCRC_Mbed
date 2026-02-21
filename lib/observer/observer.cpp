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

    // --- P2, AUFGABE 1.5 ---
    A << -905.387f, 452.694f, 452.694f, -452.694f;
    B << 452.694f, 0.000f;
    C << 0.000f, 1.000f;
    H << 30025.468f, 4641.919f;
}

observer::~observer() {}

// calculate one step of the observer
void observer::do_step(float u, float y_meas)
{
    // dxdt_hat = ...;
    // x_hat += ...;

    // --- P2, AUFGABE 1.5 ---
    dxdt_hat = A * x_hat + B * u + H * (y_meas - C * x_hat);
    x_hat += Ts * dxdt_hat;
}

// get the observed states
Matrix<float, N, 1> observer::get_x_obsv() { return x_hat; }
