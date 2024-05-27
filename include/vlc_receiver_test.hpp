#ifndef R2D2_VLC_RECEIVER_TEST_HPP
#define R2D2_VLC_RECEIVER_TEST_HPP

#include <Arduino.h>

#include <array>
#include <deque>

#include "FreeRTOS.h"
#include "test_bytes.hpp"
#include "vlc_receiver.hpp"

namespace rec {

class VLCReceiverTest {
public:
    VLCReceiverTest();

    void byteReceived( uint8_t &byte, uint &index );

    void printResults( uint32_t &result );

    void run();

private:
    xQueueHandle byte_queue;
    xTaskHandle this_task_handle;

    enum state_t { IDLE, BYTE, RESULTS };

    state_t state = IDLE;

    uint32_t result = 0;
};

}  // namespace rec

#endif  // R2D2_VLC_RECEIVER_TEST_HPP