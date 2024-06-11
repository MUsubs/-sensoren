#include <Arduino.h>

#include "photodiode.hpp"
#include "vlc_receiver.hpp"
#include "vlc_receiver_test.hpp"

int photo_pin = 26;

int led_pin = 3;
unsigned int frequency = 20;


rec::Photodiode photodiode{photo_pin, frequency};
xTaskHandle photodiode_task_handle;

rec::VLCReceiver vlc_receiver = { photodiode, frequency };
xTaskHandle vlc_receiver_task_handle;

rec::VLCReceiverTest receiver_test = { vlc_receiver };
xTaskHandle receiver_test_task_handle;


void phototdiode_task( void* pvParameters ) {
    rec::Photodiode* photodiode = static_cast<rec::Photodiode*>( pvParameters );

    photodiode->run();
    Serial.printf(
        "== INFO == Photodiode task exited, deleting task and object" );

    vTaskDelete( photodiode_task_handle );
    delete photodiode;
}

void vlc_receiver_task( void* pvParameters ) {
    rec::VLCReceiver* vlc_receiver =
        static_cast<rec::VLCReceiver*>( pvParameters );

    vlc_receiver->run();
    Serial.printf(
        "== INFO == VLCReceiver task exited, deleting task and object" );

    vTaskDelete( vlc_receiver_task_handle );
    delete vlc_receiver;
}

void vlc_receiver_test_task( void* pvParameters ) {
    rec::VLCReceiverTest* receiver_test =
        static_cast<rec::VLCReceiverTest*>( pvParameters );

    receiver_test->run();
    Serial.printf(
        "== INFO == VLCReceiverTest task exited, deleting task and object" );

    vTaskDelete( receiver_test_task_handle );
    delete receiver_test;
}

bool CreatReceiverTasks() {
    bool result = true;

    auto photodiode_task_return =
        xTaskCreate( phototdiode_task, "PHOTODIODE_RUN", 2000,
                     (void*)&photodiode, 1, &photodiode_task_handle );
    if ( photodiode_task_return != pdPASS ) {
        vTaskDelete( photodiode_task_handle );
        Serial.printf(
            "== ERROR == Creating task for photodiode failed, error code = "
            "%d\n",
            photodiode_task_return );

        result = false;
    }

    auto vlc_receiver_task_return =
        xTaskCreate( vlc_receiver_task, "VLC_RECEIVER_RUN", 2000,
                     (void*)&vlc_receiver, 1, &vlc_receiver_task_handle );
    if ( vlc_receiver_task_return != pdPASS ) {
        vTaskDelete( vlc_receiver_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_receiver failed, error code = "
            "%d\n",
            vlc_receiver_task_return );

        result = false;
    }

    auto vlc_receiver_test_task_return =
        xTaskCreate( vlc_receiver_test_task, "VLC_RECEIVER_TEST_RUN", 2000,
                     (void*)&receiver_test, 1, &receiver_test_task_handle );
    if ( vlc_receiver_test_task_return != pdPASS ) {
        vTaskDelete( receiver_test_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_receiver failed, error code = "
            "%d\n",
            vlc_receiver_test_task_return );

        result = false;
    }

    return result;
}

void setup() {
    Serial.begin( 19200 );
    // Require a serial connection to be formed, before continuing
    while ( !Serial ) {
        ;
    }
    vTaskDelay(3000);

    bool result = CreatReceiverTasks();

    if ( result == false ) {
        Serial.printf( "== INFO == Creating receiver tasks faild\n" );
    } else {
        Serial.printf( "== INFO == Created VLC Receiver tasks in setup()\n" );
    }
}

// 204, 51
std::deque<uint8_t> bytes = { 0b11001100, 0b00110011 };

void loop() {
    yield();
}