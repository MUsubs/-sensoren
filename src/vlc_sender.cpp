#include "vlc_sender.hpp"

namespace sen {

// public
VLCSender::VLCSender( int led_pin, unsigned int frequency ) :
    frequency{ frequency }, led_pin{ led_pin }, bytes{},
    bytes_queue{ xQueueCreate( 10, sizeof( std::deque<uint8_t>* ) ) } {
    pinMode( led_pin, OUTPUT );
    // bit delay in seconds
    if ( frequency <= 0 ) {
        Serial.printf(
            "== ERROR == frequency <= 0, setting it to 1 and continuing" );
        frequency = 1;
    }
    bit_delay = 1.f / (float)frequency;
}

void VLCSender::sendBytes( std::deque<uint8_t>& incoming_bytes,
                           unsigned int freq, bool overwrite_org,
                           bool update_freq ) {
    if ( update_freq ) frequency = freq;
    std::deque<uint8_t>* send_bytes;
    if ( overwrite_org ) {
        send_bytes = &incoming_bytes;
    } else {
        send_bytes = new std::deque<uint8_t>{ incoming_bytes };
    }
    xQueueSend( bytes_queue, (void*)&send_bytes, 0 );
}

void VLCSender::sendBytes( std::deque<uint8_t>& bytes, bool overwrite_org ) {
    sendBytes( bytes, frequency, overwrite_org );
}

void VLCSender::setFrequency( unsigned int new_frequency ) {
    frequency = new_frequency;
    bit_delay = 1 / frequency;
}

unsigned int VLCSender::getFrequency() {
    return frequency;
}

void VLCSender::run() {
    Serial.printf( "== INFO == Starting VLCSender run\n" );
    unsigned long start;
    unsigned long end;
    unsigned int index;
    PinStatus pin_state;

    for ( ;; ) {
        switch ( state ) {
            // state 0
            case state_t::IDLE:
                if ( bytes_queue != NULL ) {
                    if ( xQueueReceive( bytes_queue, &bytes, 0 ) ) {
                        Serial.printf( "Message received from queue\n" );
                        Serial.printf( "Address of vector in run() : %d\n",
                                       bytes );
                        index = 0;
                        state = state_t::SENDSYNC;
                        Serial.printf( "Frequency = %d | Bit Delay = %lf\n",
                                       frequency, bit_delay );
                    }
                } else {
                    Serial.printf( "== ERROR == Queue bytes_queue is NULL\n" );
                }
                break;
            // state 1
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
            // state 2
            case state_t::SENDBYTE:
                Serial.printf( "Entering SENDBYTE\n" );
                if ( index >= 8 ) {
                    Serial.printf( "End of byte reached, Resetting index\n" );
                    bytes->pop_front();
                    index = 0;
                }
                if ( bytes->size() <= 0 || bytes->empty() ) {
                    state = state_t::SENDEND;
                    break;
                }
                return_state = state_t::SENDBYTE;
                // send MSB first
                Serial.printf( "%d items in deque\n", bytes->size() );
                if ( bytes->front() )
                    Serial.printf( "Current byte value : %d\n",
                                   bytes->front() );
                Serial.printf( "Index is %d\n", index );
                pin_state = ( bytes->front() & ( 0x80 >> index ) ) ? HIGH : LOW;
                Serial.printf( "Pin state is %d\n", pin_state );
                index++;
                state = state_t::SENDBIT;
                break;
            // state 3
            case state_t::SENDEND:
                // set index to 42 for debugging purposes (this number shouldn't
                // occur any other way)
                index = 42;
                delete bytes;
                digitalWrite( led_pin, LOW );
                return_state = state_t::IDLE;
                state = state_t::IDLE;
                break;
            // state 4
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