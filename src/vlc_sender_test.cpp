#include "vlc_sender_test.hpp"

namespace sen {

VLCSenderTest::VLCSenderTest( int led_pin, unsigned int frequency ) :
    vlc_sender{VLCSender{led_pin, frequency}}
{}

void VLCSenderTest::start( bool generate_headers ) {
    std::deque<uint8_t> byte;
    for ( const uint8_t& test_byte : test_bytes ) {
        byte = { test_byte };
        if ( generate_headers ) {
            byte.push_front( vlc_sender.generateHeader( 0, byte ) );
        }
        vlc_sender.sendBytes( byte );
        vTaskDelay( 1000 );
    }
}

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