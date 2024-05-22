#ifndef R2D2_VLC_SENDER_TEST_HPP
#define R2D2_VLC_SENDER_TEST_HPP

#include <Arduino.h>

#include <array>
#include <deque>

#include "FreeRTOS.h"
#include "test_bytes.hpp"
#include "vlc_sender.hpp"

namespace sen {

class VLCSenderTest {
public:
    VLCSenderTest( VLCSender& vlc_sender );

    void start();

private:
    VLCSender& vlc_sender;
};

}  // namespace sen

#endif  // R2D2_VLC_SENDER_TEST_HPP