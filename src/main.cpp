#include <Arduino.h>

#include "vlc_sender.hpp"

int led_pin_1 = 20;
int led_pin_2 = 19;

int button_pin_1 = 7;
int button_pin_2 = 3;

long set_freq = 0;

sen::VLCSender vlc_sender_1 = { led_pin_1, 60 };
sen::VLCSender vlc_sender_2 = { led_pin_2, 120 };
xTaskHandle vlc_sender_1_task_handle;
xTaskHandle vlc_sender_2_task_handle;

void vlc_sender_1_task( void* pvParameters ) {
    sen::VLCSender* vlc_sender = static_cast<sen::VLCSender*>( pvParameters );

    vlc_sender->run();
    Serial.printf(
        "== INFO == VLCSender task exited, deleting task and object" );

    vTaskDelete( vlc_sender_1_task_handle );
    delete vlc_sender;
}

void vlc_sender_2_task( void* pvParameters ) {
    sen::VLCSender* vlc_sender = static_cast<sen::VLCSender*>( pvParameters );

    vlc_sender->run();
    Serial.printf(
        "== INFO == VLCSender task exited, deleting task and object" );

    vTaskDelete( vlc_sender_2_task_handle );
    delete vlc_sender;
}

void setup() {
    Serial.begin( 19200 );
    // Require a serial connection to be formed, before continuing
    while ( !Serial ) {
        ;
    }
    pinMode( led_pin_1, OUTPUT );
    pinMode( led_pin_2, OUTPUT );
    pinMode( button_pin_1, INPUT );
    pinMode( button_pin_2, INPUT );

    Serial.printf( "Creating VLC sender tasks\n" );

    auto vlc_sender_1_task_return =
        xTaskCreate( vlc_sender_1_task, "VLC_SENDER_RUN", 2000,
                     (void*)&vlc_sender_1, 1, &vlc_sender_1_task_handle );
    if ( vlc_sender_1_task_return != pdPASS ) {
        vTaskDelete( vlc_sender_1_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_sender_1 failed, error code = "
            "%d\n",
            vlc_sender_1_task_return );
    }
    Serial.printf( "== INFO == Created VLC Sender 1 task in setup()\n" );

    auto vlc_sender_2_task_return =
        xTaskCreate( vlc_sender_2_task, "VLC_SENDER_RUN", 2000,
                     (void*)&vlc_sender_2, 1, &vlc_sender_2_task_handle );
    if ( vlc_sender_2_task_return != pdPASS ) {
        vTaskDelete( vlc_sender_2_task_handle );
        Serial.printf(
            "== ERROR == Creating task for vlc_sender_2 failed, error code = "
            "%d\n",
            vlc_sender_2_task_return );
    }
    Serial.printf( "== INFO == Created VLC Sender 2 task in setup()\n" );
    Serial.printf( "== INFO == All VLC_Sender tasks succesfully created\n" );
}

// 204, 51
std::deque<uint8_t> bytes = { 0b11001100, 0b00110011 };
bool button_pressed_1 = false;
bool button_pressed_2 = false;

void loop() {
    if ( digitalRead( button_pin_1 ) && !button_pressed_1 ) {
        Serial.printf("== COMMAND == Sending bytes VLC_Sender 1\n");
        button_pressed_1 = true;
        vlc_sender_1.sendBytes( bytes );
    }
    if ( digitalRead( button_pin_2 ) && !button_pressed_2 ) {
        Serial.printf("== COMMAND == Sending bytes VLC_Sender 2\n");
        button_pressed_2 = true;
        vlc_sender_2.sendBytes( bytes );
    }
    if ( !digitalRead( button_pin_1 ) ) {
        button_pressed_1 = false;
    }
    if ( !digitalRead( button_pin_2 ) ) {
        button_pressed_2 = false;
    }
}