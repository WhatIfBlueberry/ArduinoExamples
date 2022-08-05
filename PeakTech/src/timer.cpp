#include <Arduino.h>
#include <Timer.h>

Timer::Timer() {
    _on = false;
}
Timer::Timer(unsigned long time) {
    _on = true;
    ttime = time;
    tlast = millis();
}
Timer::~Timer() {}

// -------------------------------------------------------------------------------- interval

void Timer::setInterval(unsigned long time) {
    ttime = time;
}

unsigned long Timer::getInterval() {
    return ttime;
}

// -------------------------------------------------------------------------------- turn on off
void Timer::start() {
    turn(true);
}

void Timer::start(unsigned long time) {
    setInterval(time);
    turn(true);
}
void Timer::stop() {
    turn(false);
}

void Timer::reset() {
    tlast = millis();
}

void Timer::turn(bool state) {
    _on = state;
    if (state)
        reset();
}

// -------------------------------------------------------------------------------- get
unsigned long Timer::delta() {
    tnow = millis();
    if (tnow < tlast)  //in case millis() has flipped
        tdelta = (0xFFFFFFFF - tlast) + tnow;
    else
        tdelta = tnow - tlast;
    return tdelta;
}

float Timer::percent() {
    if (!_on) return 0;
    delta();
    float P = (float)tdelta / ttime;
    return P;
}

bool Timer::elapsed() {
    if (!_on) return false;
    delta();
    if (tdelta >= ttime)
        return true;
    return false;
}

bool Timer::tick() {
    if (!_on) return false;
    delta();
    if (tdelta > ttime) {
        tlast = tnow;
        return true;
    }
    return false;
}

bool Timer::once() {
    if (!_on) return false;
    delta();
    if (tdelta > ttime) {
        stop();
        return true;
    }
    return false;
}
