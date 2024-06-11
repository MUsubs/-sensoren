#include "vlc_sender_test.hpp"

namespace sen {

VLCSenderTest::VLCSenderTest( VLCSender& vlc_sender ) :
    vlc_sender{ vlc_sender }, vlc_sender_task_handle{} {
    pinMode( 7, INPUT );
}

void VLCSenderTest::start( bool generate_headers ) {
    for ( ;; ) {
        if ( digitalRead( 7 ) == HIGH ) {
            vlc_sender.sendBytes( test_bytes );
        }
        vTaskDelay(1000);
    }
}

// void VLCSenderTest::start( bool generate_header ) {
//     // 204, 51, 170, 85, 240, 15
//     uint8_t frequencies[10] = {};
//     unsigned int i = 0;
//     std::deque<uint8_t> bytes = { 0b11001100, 0b00110011, 0b10101010,
//     0b01010101, 0b11110000, 0b00001111 }; for ( ;; ) {
//         if ( digitalRead( 7 ) == HIGH ) {
//             vlc_sender.setFrequency(frequencies[i]);
//             vlc_sender.sendBytes( bytes );
//             Serial.printf( "== MAIN == SENDING BYTES @ %d Hz\n",
//             frequencies[i] ); i++;
//         } else {
//         }
//         vTaskDelay( 1000 );
//     }
// }

void VLCSenderTest::statusError() {
    digitalWrite( status_led_pin, HIGH );
    vTaskDelay( 10 );
    digitalWrite( status_led_pin, LOW );
}

void VLCSenderTest::statusSending() {
    digitalWrite( status_led_pin, HIGH );
    vTaskDelay( 100 );
    digitalWrite( status_led_pin, LOW );
}

void VLCSenderTest::statusReady() {
    digitalWrite( status_led_pin, HIGH );
}

}  // namespace sen