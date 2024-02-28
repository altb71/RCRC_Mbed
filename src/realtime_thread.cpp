#include <cstdint>
#include "realtime_thread.h"
#include "DataLogger.h"
#include "GPA.h"

extern DataLogger myDataLogger;
extern GPA myGPA;

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
  float tim;
  float u_out, soll, stell;
  while (1)
    {
    ThisThread::flags_wait_any(threadFlag);
// --------------------- THE LOOP -----------------------------------------
    // Zeitverhalten
    tim = ti.read();
    soll = myDataLogger.get_set_value(tim);
    m_io->write_aout(u_out);
    myDataLogger.write_to_log(tim, u_out, m_io->read_ain1(), m_io->read_ain2());
    
    /* Regler
    soll = myDataLogger.get_set_value(tim);
    stell = 4*(soll-m_io->read_ain2());
    myDataLogger.write_to_log(tim, soll, stell, m_io->read_ain2()); 
    m_io->write_aout(stell);*/

    /* GPA
    u_out = myGPA.update(u_out, m_io->read_ain2()); */
    

// read RCRC-Output with: ... = m_io->read_ain2();
// here, you add your specific code, running in real-time (e.g. a controller)

    } // endof the main loop
}

// ----------------------------------------------------------------------------
void realtime_thread::sendSignal() { thread.flags_set(threadFlag); }

void realtime_thread::start_loop(void)
{
  thread.start(callback(this, &realtime_thread::loop));
  ticker.attach(callback(this, &realtime_thread::sendSignal), Ts);
}
