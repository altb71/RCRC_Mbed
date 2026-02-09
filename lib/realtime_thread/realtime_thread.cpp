#include "realtime_thread.h"

#include <chrono>
#include <cstdint>

#include "DataLogger.h"
#include "GPA.h"

extern DataLogger myDataLogger;
extern GPA myGPA;

using namespace Eigen;
using namespace std::chrono;

// contructor for realtime_thread loop
realtime_thread::realtime_thread(IO_handler *io, float Ts)
    : thread(osPriorityHigh1, 1024)
{
    this->Ts = Ts;   // the sampling time
    this->m_io = io; // a pointer to the inputs/outputs
    ti.reset();
    ti.start();
    u_out = 0.0f;
}

// decontructor for controller loop
realtime_thread::~realtime_thread() {}

// ----------------------------------------------------------------------------
// this is the main loop called every Ts with high priority
void realtime_thread::loop(void)
{
    float tim, w, V, u, y1, y2;
    Matrix<float, 1, 2> K2;
    Vector2f x;
    K2 << 1.41800f, 7.34136f;
    V = 9.7594f;
    observer obsv(Ts);
    // AUFGABE 1.4
    while (true) {
        ThisThread::flags_wait_any(threadFlag);
        tim = 1e-6f * (float)(duration_cast<microseconds>(ti.elapsed_time()).count());
        // --------------------- THE LOOP -----------------------------------------
        w = myDataLogger.get_set_value(tim); // get set values from datalogger
        // y1 = m_io->read_ain1();                 // read 2nd voltage at RRCRC
        y2 = m_io->read_ain2();         // read 2nd voltage at RRCRC
        obsv.do_step(u_out, y2);        // run observer, perform 1 step
        x = obsv.get_x_obsv();          // get observed values, use for ss-cntrl.
        u_out = V * w - K2 * x;         // calc. set values (ss-cntrl)
        u_out = saturate(u_out, -1.0f, 1.0f); // limit the setvalue to +-1 (mainly for the observer!)
        m_io->write_aout(u_out);        // write to analog output

        // AUFGABE 1.4
        // Aufgabe 2.6
        myDataLogger.write_to_log(tim, w, x(1), y2);
        // // GPA
        // u_out = myGPA.update(u_out, m_io->read_ain2());

    } // endof the main loop
}

float realtime_thread::saturate(float x, float ll, float ul)
{
    if (x > ul)
        return ul;
    else if (x < ll)
        return ll;
    return x;
}

// ----------------------------------------------------------------------------
void realtime_thread::sendSignal() { thread.flags_set(threadFlag); }

void realtime_thread::start_loop(void)
{
    thread.start(callback(this, &realtime_thread::loop));
    ticker.attach(callback(this, &realtime_thread::sendSignal), microseconds{static_cast<int64_t>(Ts * 1e6f)});
}
