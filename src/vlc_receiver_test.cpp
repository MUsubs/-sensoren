#include "vlc_receiver_test.hpp"

namespace rec {

VLCReceiverTest::VLCReceiverTest( VLCReceiver &vlc_receiver ) :
    vlc_receiver{ vlc_receiver },
    byte_queue{ xQueueCreate( 80, sizeof( uint8_t ) ) },
    this_task_handle{} {
    vlc_receiver.addListener( this );
}

void VLCReceiverTest::byteReceived( uint8_t &byte ) {
    // Serial.printf("Byte received: %d\n", byte);
    uint8_t value = byte;
    xQueueSend( byte_queue, &value, 0 );
}

void VLCReceiverTest::printResults( uint32_t &result ) {
    // Create a mask with the highest bit set
    uint32_t mask = 1UL << TEST_BYTES_N-1;
    Serial.print( "Test Results : " );

    // Iterate through each bit
    for ( int i = 0; i < TEST_BYTES_N; i++ ) {
        // Check if the current bit is set and print '1' or '0'
        if ( result & mask ) {
            Serial.print( '1' );
        } else {
            Serial.print( '0' );
        }
        // Shift the mask to the right for the next bit
        mask >>= 1;
    }

    Serial.print( ".\n" );
}

void printbyte( uint8_t &result ) {
    // Create a mask with the highest bit set
    uint8_t mask = 1UL << 7;
    Serial.print( "byte : " );

    // Iterate through each bit
    for ( int i = 0; i < 8; i++ ) {
        // Check if the current bit is set and print '1' or '0'
        if ( result & mask ) {
            Serial.print( '1' );
        } else {
            Serial.print( '0' );
        }
        // Shift the mask to the right for the next bit
        mask >>= 1;
    }

    Serial.print( ".\n" );
}

void VLCReceiverTest::run() {
    uint8_t received_byte = 0;
    for ( ;; ) {
        switch ( state ) {
            case IDLE:
                if ( byte_queue != NULL ) {
                    if ( uxQueueMessagesWaiting( byte_queue ) >=
                         TEST_BYTES_N ) {
                        state = BYTE;
                    }
                }
                break;

            case BYTE:
                for ( unsigned int index = 0; index < TEST_BYTES_N; index++ ) {
                    xQueueReceive( byte_queue, &received_byte, 0 );
                    if ( received_byte == sen::test_bytes[index] ) {
                        result <<= 1;
                        result |= 1;
                    } else {
                        result <<= 1;
                    }
                    Serial.printf( "Byte = %d\n", received_byte );
                }

                Serial.println( "TO RESULTS" );
                state = RESULTS;
                break;

                // printbyte(received_byte);
                // state = IDLE;

            case RESULTS:
                Serial.println( "Results state" );
                // printResults( result );
                printResults( result );
                result = 0;
                state = IDLE;

                break;

            default:
                break;
        }
    }
}

}  // namespace rec