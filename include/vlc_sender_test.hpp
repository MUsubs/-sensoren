#ifndef R2D2_VLC_SENDER_TEST_HPP
#define R2D2_VLC_SENDER_TEST_HPP

#include <array>
#include <deque>

#include "Arduino.h"
#include "FreeRTOS.h"
#include "test_bytes.hpp"
#include "vlc_sender.hpp"

namespace sen {

class VLCSenderTest {
public:
    VLCSenderTest( VLCSender& vlc_sender );
    void start( bool generate_headers = false );

private:
    VLCSender& vlc_sender;
    int status_led_pin;
    xTaskHandle vlc_sender_task_handle;

    void vlc_sender_task( void* pvParameters );

    void statusError();
    void statusSending();
    void statusReady();
};

}  // namespace sen

#endif  // R2D2_VLC_SENDER_TEST_HPP