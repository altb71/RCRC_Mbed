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
realtime_thread::realtime_thread(IO_handler *io_handler, float Ts)
    : m_Thread(osPriorityHigh1, 1024)
{
    m_Ts = Ts;                 // the sampling time
    m_IO_handler = io_handler; // a pointer to the io handler
    m_Timer.reset();
    m_Timer.start();
}

// decontructor for controller loop
realtime_thread::~realtime_thread() {}

// this is the main loop called every Ts with high priority
void realtime_thread::loop(void)
{
    // control signal, needs to be persistent within loop
    float u(0.0f);

    // --- P1, AUFGABE 1.8 ---
    // excitation signal for frequency response measurement
    float exc(0.0f);

    // --- P2, AUFGABE 1.4 ---
    // variables for state feedback controller
    Matrix<float, 1, 2> K(1.4180f, 7.3414f);
    Matrix<float, 2, 1> x(0.0f, 0.0f);

    // --- P2, AUFGABE 1.5 ---
    // feedforward term for state feedback controller
    const float V = 9.7594f;

    // --- P2, AUFGABE 2.5 ---
    // observer and variables for observer
    observer obsv(m_Ts);
    Matrix<float, 2, 1> x_hat(0.0f, 0.0f);

    while (true) {
        ThisThread::flags_wait_any(m_ThreadFlag);
        const float time = 1e-6f * (float)(duration_cast<microseconds>(m_Timer.elapsed_time()).count());
        // --------------------- THE LOOP ---------------------

        float w = myDataLogger.get_set_value(time); // get set values from the GUI

        const float y1 = m_IO_handler->read_ain1(); // read 1st voltage
        const float y2 = m_IO_handler->read_ain2(); // read 2nd voltage

        u = w + 1.0f + exc;

        m_IO_handler->write_aout(u); // write to analog output
        myDataLogger.write_to_log(time, u, y1, 0.0f, 0.0f, 0.0f, 0.0f);

        exc = myGPA.update(exc, y1); // GPA calculates future excitation exc(k+1)
    }
}

float realtime_thread::saturate(float x, float ll, float ul)
{
    if (x > ul)
        return ul;
    else if (x < ll)
        return ll;
    return x;
}

void realtime_thread::sendSignal() { m_Thread.flags_set(m_ThreadFlag); }

void realtime_thread::start_loop(void)
{
    m_Thread.start(callback(this, &realtime_thread::loop));
    m_Ticker.attach(callback(this, &realtime_thread::sendSignal), microseconds{static_cast<int64_t>(m_Ts * 1e6f)});
}
