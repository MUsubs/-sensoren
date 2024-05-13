#include <Arduino.h>

#include "vlc_sender.hpp"

int led_pin = LED_BUILTIN;
long set_freq = 0;

sen::VLCSender vlc_sender = { led_pin, 40 };
xTaskHandle vlc_sender_task_handle;

void vlc_sender_task( void* pvParameters ) {
    sen::VLCSender* vlc_sender = static_cast<sen::VLCSender*>( pvParameters );

    vlc_sender->run();
    Serial.printf(
        "== INFO == VLCSender task exited, deleting task and object" );

    vTaskDelete( vlc_sender_task_handle );
    delete vlc_sender;
}

void setup() {
    Serial.begin( 19200 );
    // Require a serial connection to be formed, before continuing
    while ( !Serial ) {
        ;
    }
    pinMode( 26, INPUT );
    Serial.printf( "Creating VLC sender task\n" );

    auto vlc_sender_task_return =
        xTaskCreate( vlc_sender_task, "VLC_SENDER_RUN", 2000,
                     (void*)&vlc_sender, 1, &vlc_sender_task_handle );
    if ( vlc_sender_task_return != pdPASS ) {
        vTaskDelete( vlc_sender_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_sender failed, error code = "
            "%d\n",
            vlc_sender_task_return );
    }
    Serial.printf( "== INFO == Created VLC Sender task in setup()\n" );
}

// 204, 51
std::deque<uint8_t> bytes = { 0b11001100, 0b00110011 };

void loop() {
    if ( digitalRead( 26 ) == HIGH ) {
        vlc_sender.sendBytes( bytes );
        Serial.printf( "== MAIN == SENDING BYTES\n" );
    } else {
    }
    vTaskDelay( 1000 );
}