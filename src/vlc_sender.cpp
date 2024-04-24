#include "vlc_sender.hpp"

namespace sen {

// public
VLCSender::VLCSender( int led_pin, unsigned int frequency ) :
    frequency{ frequency }, led_pin{ led_pin }, bytes{}, bytes_backup{},
    bytes_queue{ xQueueCreate( 10, sizeof( std::deque<uint8_t>* ) ) } {
    pinMode( led_pin, OUTPUT );
    // bit delay in seconds
    if ( frequency <= 0 ) {
        Serial.printf( "== ERROR == frequency <= 0, setting it to 1 and continuing" );
        frequency = 1;
    }
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
    bit_delay = 1 / frequency;
}

unsigned int VLCSender::getFrequency() {
    return frequency;
}

void VLCSender::run() {
    Serial.printf( "== INFO == Starting VLCSender run" );
    unsigned long start;
    unsigned long end;
    unsigned int index;
    PinStatus pin_state;

    for ( ;; ) {
        switch ( state ) {
            case state_t::IDLE:
                // wait for send event flag
                if ( bytes_queue != NULL ) {
                    if ( xQueueReceive( bytes_queue, bytes, 0 ) ) {
                        // set index to 0
                        index = 0;
                        bytes_backup = *bytes;
                        // change to SENDSYNC
                        state = state_t::SENDSYNC;
                    }
                } else {
                    Serial.printf( "== ERROR == Queue bytes_queue is NULL" );
                }
                break;
            case state_t::SENDSYNC:
                return_state = state_t::SENDSYNC;
                if ( index < 8 ) {
                    pin_state = ( index % 2 == 0 ) ? HIGH : LOW;
                    index++;
                    state = state_t::SENDBIT;
                } else {
                    index = 0;
                    state = state_t::SENDBYTE;
                }
                break;
            case state_t::SENDBYTE:
                if ( bytes->empty() ) state = state_t::SENDEND;
                if ( index >= 8 ) {
                    bytes->pop_front();
                    index = 0;
                }
                return_state = state_t::SENDBYTE;
                // send MSB first
                pin_state = ( bytes->front() & ( 0x80 >> index ) ) ? HIGH : LOW;
                index++;
                state = state_t::SENDBIT;
                break;
            case state_t::SENDEND:
                // set index to 42 for debugging purposes (this number shouldn't
                // occur any other way)
                index = 42;
                return_state = state_t::IDLE;
                state = state_t::IDLE;
                break;
            case state_t::SENDBIT:
                start = micros();
                end = start + bit_delay * 1'000'000;
                // technically this blocking loop isn't allowed
                // for testing sake, I'm keeping it in for now (as of 24 apr)
                // if this becomes the final communication protocol,
                // the loop will be subsituted with states
                while ( micros() <= end ) {
                    digitalWrite( led_pin, pin_state );
                }
                state = return_state;
                break;
            default:
                state = state_t::IDLE;
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