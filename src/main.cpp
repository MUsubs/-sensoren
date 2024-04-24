#include <Arduino.h>

#include "vlc_sender.hpp"

int led_pin = 11;
long set_freq = 0;

sen::VLCSender vlc_sender = { led_pin, 1 };
xTaskHandle vlc_sender_task_handle;

void vlc_sender_task( void* pvParameters ) {
    sen::VLCSender* vlc_sender = static_cast<sen::VLCSender*>( pvParameters );

    vlc_sender->run();

    vTaskDelete( vlc_sender_task_handle );
    delete vlc_sender;
}

void setup() {
    Serial.begin( 9600 );
    // if ( Serial.available() ) {
    //     Serial.println(
    //         "What frequency would you like to initialize sending on?\n \
    //         (this can be changed later)" );
    //     set_freq = Serial.parseInt();
    // }

    auto vlc_sender_task_return =
        xTaskCreate( vlc_sender_task, "VLC_SENDER_RUN", 2000,
                     (void*)&vlc_sender, 1, &vlc_sender_task_handle );
    if ( vlc_sender_task_return != pdPASS ) {
        vTaskDelete( vlc_sender_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_sender failed, error code = %d",
            vlc_sender_task_return );
    }
}

void loop() {
    std::deque<uint8_t> bytes = { 0b11001100, 0b00110011 };
    vlc_sender.sendBytes( bytes );
}