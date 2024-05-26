#ifndef R2D2_MESSAGE_DECODER_HPP
#define R2D2_MESSAGE_DECODER_HPP

#include "ir_receiver.hpp"
#include "ir_constants.hpp"

namespace sen {

class MessageListener {
public:
    virtual void messageReceived( uint32_t msg ) = 0;
};

class MessageDecoder : public IrListener {
public:
    MessageDecoder( IrReceiver & ir );
    void signalDetected( uint32_t us ) override;
    void pauseDetected( uint32_t us ) override;
    void setMessageListener( MessageListener * message_listener );
private:
    enum {
        wait_for_lead_signal,
        wait_for_lead_pause,
        wait_for_bit_pause
    } _state = wait_for_lead_signal;
    MessageListener * _message_listener;
    uint32_t _message;
};

}


#endif