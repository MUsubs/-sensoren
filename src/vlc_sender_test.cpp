#include "vlc_sender_test.hpp"

namespace sen {

VLCSenderTest::VLCSenderTest( VLCSender& vlc_sender ) :
    vlc_sender{ vlc_sender } {
}

void VLCSenderTest::start() {
    std::deque<uint8_t> byte;
    for ( const uint8_t& test_byte : test_bytes ) {
        byte = { test_byte };
        vlc_sender.sendBytes( byte );
        vTaskDelay(1000);
    }
}

}  // namespace sen