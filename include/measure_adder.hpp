#ifndef MEASURE_ADDER_HPP
#define MEASURE_ADDER_HPP

#include "serial_control.hpp"

namespace sen {
    enum sens_t { TEMP };

	/**
	* @class MeasureAdder measure_adder.hpp "include/measure_adder.hpp"
	* @brief Class for adding measurements to the buffer.
	* @details This class is used to add measurements to the buffer. The buffer is used to store the measurements before sending them to the server.
	*/
    class MeasureAdder {
        public:
			/**
			* @brief Constructor for the MeasureAdder class.
			* @details This constructor initializes the serialControl object.
			*
			* @param data_sender Reference to the DummyDataSender object.
			* @param task_priority Priority of the task.
			* @return MeasureAdder object
			* @see SerialControl::SerialControl
			*/
            MeasureAdder(DummyDataSender& data_sender, int task_priority);

			/**
			* @brief This function adds a measurement to the buffer.
			* @details This function is used to add a measurement to the buffer. The buffer is used to store the measurements before sending them to the server.
			* 
			* @param sensor_type Type of the sensor.
			* @param value Value of the sensor.
			* @return void
			* @see SerialControl::addMeasure
			*/
            void receivedSENS(sens_t sensor_type, float value);
		
		private:
            SerialControl serial_control;
    };

} // namespace sen

#endif // MEASURE_ADDER_HPP