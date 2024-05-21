#ifndef R2D2_VLC_RECEIVER_HPP
#define R2D2_VLC_RECEIVER_HPP

#include <Arduino.h>
#include <deque>

#include "FreeRTOS.h"
#include "queue.h"
#include "photodiode.hpp"

namespace rec {

class VLCReceiver : PhotodiodeListener{

public:
    VLCReceiver( int phot_pin, unsigned int frequency = 60 );

    void pauseDetected( uint32_t pause );

    bool createMessage( uint16_t &message, int &pause );

    bool isValidCheckSum( const uint16_t &message );

    void run();

private:
    int phot_pin;
    unsigned int frequency;
    double bit_delay;
    xQueueHandle pause_queue;
    xTaskHandle this_task_handle;
    PinStatus pin_state; 
    int low_threshold;
    int high_threshold;
    
    enum state_t{ IDLE, SYNC, SIGNAL, MESSAGE};

    state_t state = IDLE;

};

} // namespace rec

#endif  // R2D2_VLC_RECEIVER_HPP