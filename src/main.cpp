#include <Arduino.h>

#include "vlc_sender.hpp"
#include "vlc_sender_test.hpp"

xTaskHandle vlc_sender_test_task_handle;

void vlc_sender_test_task(void* pvParameters){
    sen::VLCSenderTest vlc_sender_test(3, 60);
    vlc_sender_test.start();
}

void setup() {
    Serial.begin( 19200 );
    // Require a serial connection to be formed, before continuing
    while ( !Serial ) {
        ;
    }
    // wait for 2 seconds to let serial initialize
    vTaskDelay(2000);
    Serial.printf( "== INIT == Creating VLC sender test task\n" );

    auto vlc_sender_task_return =
        xTaskCreate( vlc_sender_test_task, "VLC_SENDER_TEST", 2000,
                     NULL, 1, &vlc_sender_test_task_handle );
    if ( vlc_sender_task_return != pdPASS ) {
        vTaskDelete( vlc_sender_test_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_sender_test failed, error code = "
            "%d\n",
            vlc_sender_task_return );
    }
    Serial.printf( "== INIT-SUCCESS == Created VLC Sender task in setup()\n" );
}

void loop() {
}