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
    float time{0.0f}, w{0.0f}, y1{0.0f}, y2{0.0f}, u(0.0f), exc(0.0f);
    Matrix<float, 1, 2> K;
    K << 1.418f, 7.341f;
    Vector2f x;
    x.setZero();
    float V = 9.7594f;
    Vector2f x_hat;
    x_hat.setZero();
    observer obsv(m_Ts);

    while (true) {
        ThisThread::flags_wait_any(m_ThreadFlag);
        time = 1e-6f * (float)(duration_cast<microseconds>(m_Timer.elapsed_time()).count());
        // --------------------- THE LOOP ---------------------

        w = myDataLogger.get_set_value(time); // get set values from the GUI

        y1 = m_IO_handler->read_ain1(); // read 1st voltage
        y2 = m_IO_handler->read_ain2(); // read 2nd voltage

        // m_IO_handler->write_aout(w); // write to analog output

        // // --- P1, AUFGABE 1.11 ---
        // myDataLogger.write_to_log(time, w, y1, y2, 0.0f, 0.0f, 0.0f);

        // // --- P1, AUFGABE 1.12 ---
        // u = 4.0f * (w - y2);          // simple P controller, gain = 4.0f
        // u = saturate(u, -1.0f, 1.0f); // limit the setvalue to +-1
        // m_IO_handler->write_aout(u);  // write to analog output
        // myDataLogger.write_to_log(time, w, y1, y2, u, 0.0f, 0.0f);

        // // --- P2, AUFGABE 1.4 ---
        // x << y1, y2;                  // combine to vector
        // // u = w - K * x;                // calculate control signal (systems-input)
        // u = V * w - K * x;            // calculate control signal (systems-input)
        // u = saturate(u, -1.0f, 1.0f); // limit the setvalue to +-1
        // m_IO_handler->write_aout(u);  // write to analog output
        // myDataLogger.write_to_log(time, w, y1, y2, u, 0.0f, 0.0f);

        // --- P2, AUFGABE 1.5 ---
        // x << y1, y2;                  // combine to vector
        // u = V * w - K * x;            // calculate control signal (systems-input)
        // u = saturate(u, -1.0f, 1.0f); // limit the setvalue to +-1
        obsv.do_step(u, y2);          // run observer, perform 1 step
        x_hat = obsv.get_x_obsv();    // get observed values
        u = V * w - K * x_hat;        // calculate control signal (systems-input)
        u = saturate(u, -1.0f, 1.0f); // limit the setvalue to +-1
        m_IO_handler->write_aout(u);  // write to analog output
        myDataLogger.write_to_log(time, w, y1, y2, u, x_hat(0), x_hat(1));

        // --- P1, AUFGABE 1.8 ---
        // GPA - do not overwrite exc if you want to excite via the GPA
        // m_IO_handler->write_aout(exc); // write to analog output
        exc = myGPA.update(w, y2); // GPA calculates future excitation exc(k+1)
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
