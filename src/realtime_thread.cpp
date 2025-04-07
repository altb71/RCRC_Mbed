#include <chrono>
#include <cstdint>
#include "realtime_thread.h"
#include "DataLogger.h"
#include "GPA.h"

extern DataLogger myDataLogger;
extern GPA myGPA;
using namespace Eigen;
using namespace std::chrono;
// contructor for realtime_thread loop
realtime_thread::realtime_thread(IO_handler *io,float Ts) : thread(osPriorityHigh1, 1024)
{
  this->Ts = Ts;        // the sampling time
  this->m_io = io;      // a pointer to the inputs/outputs
  ti.reset();
  ti.start();
}

// decontructor for controller loop
realtime_thread::~realtime_thread() {}

// ----------------------------------------------------------------------------
// this is the main loop called every Ts with high priority
void realtime_thread::loop(void)
{
  float tim,w,V,u,x1,x2,y;
  Matrix<float,1,2> K2;
  // AUFGABE 1.4
  while (1)
    {
    ThisThread::flags_wait_any(threadFlag);
    tim = 1e-6*(duration_cast<microseconds>(ti.elapsed_time()).count());
// --------------------- THE LOOP -----------------------------------------
    w = myDataLogger.get_set_value(tim);    // get set values from datalogger
    u_out = w;
    m_io->write_aout(u_out);                // write to analog output
    x1 = m_io->read_ain1();                 // read 2nd voltage at RRCRC
    x2 = m_io->read_ain2();                 // read 2nd voltage at RRCRC
    // AUFGABE 1.4
    // Aufgabe 2.6
    
    
    myDataLogger.write_to_log(tim,w,x1,x2);
    /* GPA
    u_out = myGPA.update(u_out, m_io->read_ain2()); */

    } // endof the main loop
}

// ----------------------------------------------------------------------------
void realtime_thread::sendSignal() { thread.flags_set(threadFlag); }

void realtime_thread::start_loop(void)
{
  thread.start(callback(this, &realtime_thread::loop));
  ticker.attach(callback(this, &realtime_thread::sendSignal), Ts);
}
