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

   			void receivedINST( inst_t inst_type, std::array<uint8_t, 3>& data ) override;
		    void receivedINST( InstPacket_t& inst_p ) override; 
		    void receivedINST( inst_t inst_type ) override;
    		void receivedUPDATE( UpdatePacket_t& update_p ) override;
		    void receivedUPDATE( data_t data_type, std::array<uint8_t, 3>& data ) override;
			void receivedSENS( sens_t sensor, float data ) override;


		private:
            SerialControl& serial_control;

    };

} // namespace sen

#endif // MESSAGE_PASSER_HPP