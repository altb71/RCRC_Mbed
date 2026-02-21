#include "LinearCharacteristics.h"

using namespace std;

LinearCharacteristics::LinearCharacteristics(float gain, float offset)
{
    m_gain = gain;
    m_offset = offset;
    m_ulim = 999999.0f;  // a large number
    m_llim = -999999.0f; // a large neg. number
}

// --- P1, AUFGABE 1.5 ---
LinearCharacteristics::LinearCharacteristics(float xmin, float xmax, float ymin, float ymax)
{
    m_gain = (ymax - ymin) / (xmax - xmin);
    m_offset = xmin - ymin / m_gain;
    m_ulim = ymax;
    m_llim = ymin;
}

LinearCharacteristics::~LinearCharacteristics() {}

float LinearCharacteristics::evaluate(float x)
{
    // calculate result as y(x) = gain * (x - offset)
    // return x;

    // --- P1, AUFGABE 1.3 ---
    float ret_val = m_gain * (x - m_offset);

    // --- P1, AUFGABE 1.4 ---
    if (ret_val > m_ulim)
        ret_val = m_ulim;
    else if (ret_val < m_llim)
        ret_val = m_llim;
    return ret_val;
}

void LinearCharacteristics::set_limits(float ll, float ul)
{
    m_llim = ll;
    m_ulim = ul;
}
