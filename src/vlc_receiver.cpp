#include "vlc_receiver.hpp"
#include "vlc_sender.hpp"

#define QUEUE_LENGTH 32

namespace rec {

void VLCReceiver::addListener( ReceiverListener *listener ) {
    if ( currentNumberOfListeners < maxNumberOfListeners ) {
        ReceiverListenerArr[currentNumberOfListeners] = listener;
        currentNumberOfListeners++;
    }
}

VLCReceiver::VLCReceiver( unsigned int frequency) :
    frequency{ frequency },
    pulse_length_queue{ xQueueCreate( QUEUE_LENGTH, sizeof( double ) ) },
    this_task_handle{} {
    if ( frequency <= 0 ) {
        Serial.printf(
            "== ERROR == frequency <= 0, setting it to 1 and continuing" );
        frequency = 1;
    }
    bit_delay = 1.f / (float)frequency;
}

void VLCReceiver::pulseDetected( double pulse_length ) {
    double value = pulse_length;
    xQueueSend( pulse_length_queue, &value, 0 );
}

bool VLCReceiver::readHeader( uint8_t &header, double &pulse_length ) {
    for ( unsigned int i = 0; i < 8; i++ ) {
        if ( pulse_length > ( ( bit_delay / 2 ) + 0.003 ) &&
             pulse_length <= ( bit_delay + 0.003 ) ) {
            header |= 1;
            header <<= 1;

        } else if ( pulse_length > 0 &&
                    pulse_length < ( ( bit_delay / 2 ) + 0.003 ) ) {
            header |= 0;
            header <<= 1;
        }

        if ( i >= 7 ) return true;
        if ( !xQueueReceive( pulse_length_queue, &pulse_length, 0 ) ) {
            return false;
        }
    }
    return true;
}

bool VLCReceiver::readByte( uint8_t &message, double &pulse_length ) {
    for ( unsigned int i = 0; i < 8; i++ ) {
        if ( pulse_length > ( ( bit_delay / 2 ) + 0.003 ) &&
             pulse_length <= ( bit_delay + 0.003 ) ) {
            message |= 1;
            message <<= 1;

        } else if ( pulse_length > 0 &&
                    pulse_length < ( ( bit_delay / 2 ) + 0.003 ) ) {
            message <<= 1;
        }

        if ( i >= 7 ) return true;
        if ( !xQueueReceive( pulse_length_queue, &pulse_length, 0 ) ) {
            return false;
        }
    }
    return true;
}

bool VLCReceiver::isValidCheckSum( const uint16_t &message ) {
    // is it necessary to check if the massage is valid?
    return true;
}

void VLCReceiver::run() {
    double pulse_length = 0;
    unsigned int index = 0;

    uint8_t header = 0;
    uint8_t byte = 0;

    uint8_t message[10];  // temporary only 10 bytes long

    for ( ;; ) {
        switch ( state ) {
            case IDLE:

                if ( pulse_length_queue != NULL ) {
                    if ( xQueueReceive( pulse_length_queue, &pulse_length,
                                        0 ) ) {
                        state = HEADER;
                    }
                }

                break;

            case HEADER:

                readHeader( header, pulse_length );

                state = MESSAGE;

                break;

            case MESSAGE:

                if ( xQueueReceive( pulse_length_queue, &pulse_length, 0 ) ) {
                    readByte( byte, pulse_length );
                }

                message[0] = byte;

                byte = 0;
                header = 0;

                state = IDLE;

                break;
        }
    }
}

}  // namespace rec