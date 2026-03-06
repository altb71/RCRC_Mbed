#pragma once

class LinearCharacteristics
{
public:
    LinearCharacteristics() {};          // default constructor
    LinearCharacteristics(float, float); // constructor with gain and offset

    float evaluate(float); // calculate y(x)
    float operator()(float x) { return evaluate(x); }
    virtual ~LinearCharacteristics(); // deconstructor
    void set_limits(float, float);

private:
    float m_gain;
    float m_offset;
    float m_ulim;
    float m_llim;
};
