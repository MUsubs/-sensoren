#include "measure_adder.hpp"

namespace sen {
    MeasureAdder::MeasureAdder(DummyDataSender& data_sender, int task_priority) : serial_control(data_sender, task_priority) {}

    void MeasureAdder::receivedSENS(sens_t sensor_type, float value) {
        serial_control.addMeasure(value);
    }

} // namespace sen