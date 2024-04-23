#ifndef R2D2_VLC_SENDER_HPP
#define R2D2_VLC_SENDER_HPP

#include <Arduino.h>

#include <deque>

#include "FreeRTOS.h"
#include "queue.h"

namespace sen {

class VLCSender {
public:
    VLCSender( int led_pin, unsigned int frequency = 60 );

    // sendBytes method with frequency parameter takes frequency from parameter,
    // doesn't overwrite private variable by default, set update_freq to true to
    // overwrite
    void sendBytes( std::deque<uint8_t>& bytes, unsigned int freq,
                    bool update_freq = false );

    // sendBytes method without frequency parameter
    // takes frequency from private variable
    void sendBytes( std::deque<uint8_t>& bytes );

    void setFrequency( unsigned int new_frequency );
    unsigned int getFrequency();
    void run();

private:
    unsigned int frequency;
    unsigned int bit_delay;
    int led_pin;
    unsigned int index;
    std::deque<uint8_t>* bytes;
    xQueueHandle bytes_queue;

    uint8_t generateChecksum( std::deque<uint8_t>& bytes );
    void sendByte( uint8_t byte );
    void sendBit( bool bit );

    enum class state_t { IDLE, SENDSYNC, SENDBYTE, SENDEND, SENDONE, SENDZERO };

    state_t state = IDLE;
    state_t return_state = IDLE;
};

}  // namespace sen

#endif  // R2D2_VLC_SENDER_HPP