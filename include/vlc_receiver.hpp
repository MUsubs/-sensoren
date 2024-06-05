#ifndef R2D2_VLC_RECEIVER_HPP
#define R2D2_VLC_RECEIVER_HPP

#include <Arduino.h>

#include <deque>

#include "FreeRTOS.h"
#include "photodiode.hpp"
#include "queue.h"

namespace rec {

class ReceiverListener {
public:
    virtual void byteReceived( uint8_t& byte ) = 0;
};

class VLCReceiver : PhotodiodeListener {
public:
    VLCReceiver( Photodiode &photodiode, unsigned int frequency = 60 );

    void addListener( ReceiverListener *listener );

    void pulseDetected( double pulse_length ) override;

    void resetQueue() override;

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

    Photodiode &photodiode;

    enum state_t { IDLE, HEADER, MESSAGE };

    state_t state = IDLE;

    static const unsigned int maxNumberOfListeners = 1;

    std::array<ReceiverListener *, maxNumberOfListeners> ReceiverListenerArr;
    unsigned int currentNumberOfListeners = 0;
};

}  // namespace rec

#endif  // R2D2_VLC_RECEIVER_HPP