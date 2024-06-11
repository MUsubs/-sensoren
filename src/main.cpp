#include <Arduino.h>

#include "vlc_sender.hpp"
#include "vlc_sender_test.hpp"

xTaskHandle vlc_sender_test_task_handle;
xTaskHandle vlc_sender_task_handle;

sen::VLCSender vlc_sender{3, 10};

void vlc_sender_test_task(void* pvParameters){
    sen::VLCSenderTest vlc_sender_test(vlc_sender);
    vlc_sender_test.start();
}

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
    // wait for 2 seconds to let serial initialize
    vTaskDelay(2000);
    Serial.printf( "== INIT == Creating VLC sender test task\n" );

    auto vlc_sender_test_task_return =
        xTaskCreate( vlc_sender_test_task, "VLC_SENDER_TEST", 2000,
                     NULL, 1, &vlc_sender_test_task_handle );
    if ( vlc_sender_test_task_return != pdPASS ) {
        vTaskDelete( vlc_sender_test_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_sender_test failed, error code = "
            "%d\n",
            vlc_sender_test_task_return );
    }
    Serial.printf( "== INIT-SUCCESS == Created VLC Sender task in setup()\n" );

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

void loop() {
}