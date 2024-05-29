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
                           unsigned int freq, bool update_freq ) {
    if ( update_freq ) frequency = freq;
    std::deque<uint8_t>* send_bytes;
    send_bytes = &incoming_bytes;
    xQueueSend( bytes_queue, (void*)&send_bytes, 0 );
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
    Serial.printf( "== INFO == Starting VLCSender run\n" );
    unsigned long start;
    unsigned long end;
    unsigned int index;
    unsigned int byte_amount;
    bool bit_state;

    for ( ;; ) {
        switch ( state ) {
            // state 0
            case state_t::IDLE:
                if ( bytes_queue != NULL ) {
                    if ( xQueueReceive( bytes_queue, &bytes, 0 ) ) {
                        Serial.printf( "Message received from queue\n" );
                        index = 0;
                        byte_amount = bytes->size();
                        state = state_t::SENDBYTE;
                        Serial.printf( "Frequency = %d | Bit Delay = %lf\n",
                                       frequency, bit_delay );
                    }
                } else {
                    Serial.printf( "== ERROR == Queue bytes_queue is NULL\n" );
                }
                break;
            // state 1
            case state_t::SENDBYTE:
                Serial.printf( "Entering SENDBYTE\n" );
                if ( index >= 8 ) {
                    Serial.printf( "End of byte reached, Resetting index\n" );
                    bytes->push_back( bytes->front() );
                    bytes->pop_front();
                    byte_amount--;
                    index = 0;
                }
                if ( byte_amount <= 0 ) {
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
                bit_state = ( bytes->front() & ( 0x80 >> index ) ) ? true : false;
                Serial.printf( "Pin state is %d\n", bit_state );
                index++;
                state = state_t::SENDBIT;
                break;
            // state 2
            case state_t::SENDEND:
                // set index to 42 for debugging purposes (this number shouldn't
                // occur any other way)
                Serial.printf( "== INFO == All bytes sent\n" );
                digitalWrite( led_pin, LOW );
                index = 42;
                byte_amount = 0;
                return_state = state_t::IDLE;
                state = state_t::IDLE;
                break;
            // state 3
            case state_t::SENDBIT:
                start = micros();
                if ( bit_state ) {
                    end = start + bit_delay * 1'000'000;
                } else {
                    end = start + bit_delay * 500'000;
                }
                while ( micros() <= end ) {
                    analogWriteFreq(38000);
                    analogWrite( led_pin, PWM_HIGH );
                    analogWriteFreq(1000);
                }
                digitalWrite( led_pin, LOW );
                end = micros() + bit_delay * 500'000;
                while ( micros() <= end );
                state = return_state;
                break;
            default:
                state = state_t::IDLE;
                break;
        }
    }
}

// header is 8 bits, format = MSB | bytes amount : 5 | ID : 3 | LSB
uint8_t VLCSender::generateHeader( uint8_t id, std::deque<uint8_t>& bytes ) {
    if ( bytes.size() > 31 ) {
        Serial.printf(
            "== ERROR == Attempting to send %d bytes, maximum allowed number "
            "is 31\n",
            bytes.size() );
        return 0;
    }
    if ( id > 7 ) {
        Serial.printf(
            "== ERROR == Attempting to send ID of %d, ID should be in range "
            "0-7 (inclusive)",
            id );
        return 0;
    }

    uint8_t header = 0;
    header |= (uint8_t)bytes.size();
    header <<= 3;
    header |= id;
    return header;
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