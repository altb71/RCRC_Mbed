#define S1 true

#include "IO_handler.h"

#include <cstdint>

// constructors
IO_handler::IO_handler(void)
    : a_out(PA_5)
    , a_in1(PA_6)
    , a_in2(PA_7)
    , d_out(PC_8)
{
    a_out.write(0.0f);
    lc_in = LinearCharacteristics(0.0f, 1.0f, -1.0f, 1.0f);
    lc_out = LinearCharacteristics(-1.0f, 1.0f, 0.0f, 1.0f);
    return;
}

IO_handler::~IO_handler() {}

float IO_handler::read_ain1(void) { return lc_in(a_in1.read()); }

float IO_handler::read_ain2(void) { return lc_in(a_in2.read()); }

void IO_handler::write_aout(float output)
{
    set_value = (output);
    a_out.write(lc_out(set_value));
}

float IO_handler::get_set_value() { return set_value; }

void IO_handler::write_dout(bool val) { d_out = val; }
