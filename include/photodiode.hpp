#ifndef PHOTODIODE_HPP
#define PHOTODIODE_HPP

#include <Arduino.h>

#include <array>
#include <deque>

#include "FreeRTOS.h"
#include "queue.h"

namespace rec {

class PhotodiodeListener {
public:
    virtual void pulseDetected( double pulse_length );
    virtual void resetQueue();
};

class Photodiode {
public:
    Photodiode( int phot_pin, unsigned int frequency );

    void addListener( PhotodiodeListener *listener );

    void run();
     
private:
    static const unsigned int maxNumberOfListeners = 1;

    int main_clock_us = 1000;

    std::array<PhotodiodeListener *, maxNumberOfListeners>
        PhotodiodeListenerArr;
    unsigned int currentNumberOfListeners = 0;
    int phot_pin;
    unsigned int frequency;
    double bit_delay;

    xTaskHandle this_task_handle;

    enum state_t { IDLE, SIGNAL };
    state_t state = IDLE;

    uint16_t low_threshold = 0;
};

}  // namespace rec

#endif  // PHOTODIODE_HPP