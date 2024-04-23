#include "vlc_sender.hpp"

namespace sen {

// public
VLCSender::VLCSender( int led_pin, unsigned int frequency ) :
    frequency{ frequency },
    led_pin{ led_pin },
    index{ 0 },
    bytes{},
    bytes_queue{ xQueueCreate( 10, sizeof( std::deque<uint8_t>* ) ) } {
    pinMode( led_pin, OUTPUT );
    // bit delay in seconds
    bit_delay = 1 / frequency;
}

void VLCSender::sendBytes( std::deque<uint8_t>& bytes, unsigned int freq,
                           bool update_freq ) {
    if ( update_freq ) frequency = freq;
    xQueueSend( bytes_queue, &bytes, 0 );
}

void VLCSender::sendBytes( std::deque<uint8_t>& bytes ) {
    sendBytes( bytes, frequency );
}

void VLCSender::setFrequency( unsigned int new_frequency ) {
    frequency = new_frequency;
}

unsigned int VLCSender::getFrequency() {
    return frequency;
}

void VLCSender::run() {
    Serial.printf("== INFO == Starting VLCSender run");
    for ( ;; ) {
        switch ( state ) {
            case state_t::IDLE:
                // wait for send event flag
                if ( bytes_queue != NULL ) {
                    if ( xQueueReceive( bytes_queue, bytes, 0 ) ) {
                        // set index to 0
                        index = 0;
                        // change to SENDSYNC
                        state = state_t::SENDSYNC;
                    }
                } else {
                    Serial.printf( "== ERROR == Queue bytes_queue is NULL" );
                }
                break;
            case state_t::SENDSYNC:
                // set return_state to SENDSYNC;
                // if index < 8 {
                // if index % 2 == 1 change to SENDONE
                // else change to SENDZERO
                // increment index
                // }
                // set index to 0
                // change to SENDBYTE
                break;
            case state_t::SENDBYTE:
                // if bytes.size == 0 change to return_state
                // set return_state to SENDBYTE
                // if (bytes.front & (1 << index)) SENDONE
                // else SENDZERO
                // increment index
                break;
            case state_t::SENDEND:
                // ???? TBD
                break;
            case state_t::SENDONE:
                unsigned long start = micros();
                unsigned long end = start + bit_delay * 1'000'000;
                while ( micros() <= end ) {
                    digitalWrite( led_pin, HIGH );
                }
                state = return_state;
                break;
            case state_t::SENDZERO:
                unsigned long start = micros();
                unsigned long end = start + bit_delay * 1'000'000;
                while ( micros() <= end ) {
                    digitalWrite( led_pin, LOW );
                }
                state = return_state;
                break;
        }
    }
}

// private
uint8_t VLCSender::generateChecksum( std::deque<uint8_t>& bytes ) {
    uint8_t result = 0;
    for ( auto byte : bytes ) {
        result ^= byte;
    }
    return result;
}

}  // namespace sen