#ifndef R2D2_VLC_RECEIVER_HPP
#define R2D2_VLC_RECEIVER_HPP

#include <Arduino.h>

#include <deque>

#include "FreeRTOS.h"
#include "photodiode.hpp"
#include "queue.h"

namespace rec {

class VLCReceiver : PhotodiodeListener {
public:
    VLCReceiver( unsigned int frequency = 60, VLCReceiverTest &test );

    void pulseDetected( double pulse_length );

    bool readByte( uint8_t &message, double &pulse_length );

    bool readHeader( uint8_t &header, double &pulse_length );

    bool isValidCheckSum( const uint16_t &message );

    void run();

private:
    int phot_pin;
    unsigned int frequency;
    double bit_delay;
    xQueueHandle pulse_length_queue;
    xTaskHandle this_task_handle;
    PinStatus pin_state;
    int low_threshold;
    int high_threshold;

    enum state_t { IDLE, HEADER, MESSAGE };

    state_t state = IDLE;

    VLCReceiverTest test;
};

}  // namespace rec

#endif  // R2D2_VLC_RECEIVER_HPP