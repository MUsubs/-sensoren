#ifndef MESSAGE_PASSER_HPP
#define MESSAGE_PASSER_HPP

#include "serial_control.hpp"
#include "message_interpreter_listener.hpp"

namespace sen {

	/**
	* @class MessagePasser message_passer.hpp "include/message_passer.hpp"
	* @brief Class for adding measurements to the buffer.
	* @details This class is used to add measurements to the buffer. The buffer is used to store the measurements before sending them to the server.
	*/
    class MessagePasser : public MessageInterpreterListener {
        public:
			/**
			* @brief Constructor for the MessagePasser class.
			* @details This constructor initializes the serialControl object.
			*
			* @param data_sender Reference to the DummyDataSender object.
			* @param task_priority Priority of the task.
			* @return MessagePasser object
			* @see SerialControl::SerialControl
			*/
            MessagePasser( DataSender data_sender, int task_priority );

		private:
            SerialControl& serial_control;
    };

} // namespace sen

#endif // MESSAGE_PASSER_HPP