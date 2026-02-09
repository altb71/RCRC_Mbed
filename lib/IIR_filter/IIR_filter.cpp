#include "IIR_filter.h"

// constructors for Integration
IIR_filter::IIR_filter(float Ts)
{
    b0 = Ts / 2.0f;
    b1 = Ts / 2.0f;
    a0 = -1.0f;
    yk = 0.0f;
    uk = 0.0f;
}

// constructors for derivative with low-pass filtering
IIR_filter::IIR_filter(float tau, float Ts)
{
    a0 = -(1.0f - Ts / tau); // 2nd version with LP-filter
    b0 = -1.0f / tau;        // see Zwischenpruefung!!!
    b1 = 1.0f / tau;
    yk = 0.0f;
    uk = 0.0f;
}

IIR_filter::IIR_filter(float tau, float Ts, float K)
{
    b0 = K * Ts / tau;
    b1 = 0.0f;
    a0 = -(1.0f - Ts / tau);
    yk = 0.0f;
    uk = 0.0f;
}

// Methods:
float IIR_filter::eval(float u)
{
    /* */
    float y_new = -a0 * yk + b1 * u + b0 * uk;
    yk = y_new;
    uk = u;
    return y_new; //
}

// Deconstructor
IIR_filter::~IIR_filter() {}
