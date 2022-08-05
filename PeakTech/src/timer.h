#pragma once

class Timer {
   public:
    Timer();
    Timer(unsigned long time);
    ~Timer();
    void start();
    void start(unsigned long time);
    void stop();
    void setInterval(unsigned long time);
    unsigned long getInterval();
    void reset();  // sets begintime to now

    void turn(bool state);
    bool isON() { return _on; };

    unsigned long delta();  // returns time im milliseconds since last reset
    float percent();        // returns the percentage of time elapsed (3 sec of 5 sec timer is 60% or .6)
    bool elapsed();         // returns true if time elapsed
    bool tick();            // returns true when Timer is exceeded and resets timer
    bool once();            // returns true when Timer is exceeded and stops timer

    bool operator()() { return tick(); }
    void operator()(const unsigned long time) { setInterval(time); }
    void operator()(const bool state) { turn(state); }
    void operator>>(unsigned long &delta) { delta = this->delta(); }
    void operator>>(float &percent) { percent = this->percent(); }

   private:
    bool _on;
    unsigned long ttime;   // time Interval for each full Timer tick
    unsigned long tlast;   // start time
    unsigned long tnow;    // current time
    unsigned long tdelta;  // timedelta between start and current
};