#include "message_passer.hpp"

namespace sen {
    MessagePasser::MessagePasser( DataSender data_sender, int task_priority ) : serial_control(data_sender, task_priority) {}

    void MessagePasser::receivedINST( inst_t inst_type, std::array<uint8_t, 3>& data ) {
        InstPacket_t inst_packet{ inst_type, data};
    }

    void MessagePasser::receivedINST( InstPacket_t& inst_p ) { 
    
    }

    void MessagePasser::receivedINST( inst_t inst_type ) {
        InstPacket_t inst_packet;
        inst_packet.inst_type = inst_type;

    }

    void MessagePasser::receivedUPDATE( UpdatePacket_t& update_p ) {
    }

    void MessagePasser::receivedUPDATE( data_t data_type, std::array<uint8_t, 3>& data ) {
        UpdatePacket_t update_packet{ data_type, data };
    }

    void MessagePasser::receivedSENS( sens_t sensor, float data ) {

    }

} // namespace sen