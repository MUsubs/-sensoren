#include "measure_passer.hpp"

namespace sen {
    MessagePasser::MessagePasser( DataSender data_sender, int task_priority ) : serial_control(data_sender, task_priority) {}
} // namespace sen