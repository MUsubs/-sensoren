#ifndef R2D2_VLC_SENDER_HPP
#define R2D2_VLC_SENDER_HPP

#include <Arduino.h>

#include <deque>

#include "FreeRTOS.h"
#include "queue.h"

namespace sen {

#define PWM_HIGH 50

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

    uint8_t generateHeader( uint8_t id, std::deque<uint8_t>& bytes );

private:
    unsigned int frequency;
    double bit_delay;
    int led_pin;
    std::deque<uint8_t>* bytes;
    xQueueHandle bytes_queue;

    uint8_t generateChecksum( std::deque<uint8_t>& bytes );

    enum class state_t { IDLE, SENDBYTE, SENDEND, SENDBIT };

    state_t state = state_t::IDLE;
    state_t return_state = state_t::IDLE;
};

}  // namespace sen

#endif  // R2D2_VLC_SENDER_HPP