#include "vlc-receiver.hpp"

#define QUEUE_LENGTH 20

namespace rec {

VLCReceiver::VLCReceiver( int phot_pin, unsigned int frequency ) :
    frequency{ frequency }, phot_pin{ phot_pin },
    pause_queue{ xQueueCreate( QUEUE_LENGTH, sizeof(uint32_t) ) },
    this_task_handle{} {

    pinMode( phot_pin, OUTPUT );

    if ( frequency <= 0 ) {
    Serial.printf(
        "== ERROR == frequency <= 0, setting it to 1 and continuing" );
    frequency = 1;
    }
    bit_delay = 1.f / (float)frequency;
    
}

void VLCReceiver::pauseDetected(uint32_t pause)
{
    uint32_t value = pause;
    xQueueSend(pause_queue, &value, 0);
}

// bool VLCReceiver::createMessage(uint16_t &message, int &pause)
// {

//     for (unsigned int i = 0; i < 16; i++)
//     {
//         // pause = pausesChannel.read();

//         if (pause > 200 && pause < 2000)
//         {
//             message |= (((pause > 1000) ? 0 : 1) << i);
//         }

//         else if (pause > 3500 && pause < 5000)
//         {
//             return false;
//         }
//     }
//     return true;
// }

bool VLCReceiver::isValidCheckSum(const uint16_t &message)
{
    // is it necessary to check if the massage is valid?
    return true;
}


void VLCReceiver::run()
{
    uint32_t pause = 0;
    bool sync_confirmed = false;
    uint8_t byte;
    uint8_t message[10]; // temporary only 10 bytes long
    unsigned int index = 0;

    for (;;)
    {
        switch (state)
        {
        case IDLE:

            if (pause_queue != NULL) {
                if (xQueueReceive(pause_queue, &pause, 0))
                {
                    state = SYNC;
                }
            }

            delayMicroseconds(20000); // 200 ms

            break;

            case SYNC:

            while (!sync_confirmed)
            {
                
                if (xQueueReceive(pause_queue, &pause, 0))
                {
                    
                    if(pause == bit_delay)
                    {
                        // two bits: 0 and 1
                    }
                    else if (pause > 3500 && pause < 5000 && sync_confirmed)
                    {
                        // long pause to confirm that the sync byte is over
                        // it should also be added in the sender!

                        state = MESSAGE;
                        break;
                    }
                    else if (pause == 0)
                    {
                        // false, sync byte does not contain 11
                    }
                }
                else
                {
                    delayMicroseconds(bit_delay);
                }
            }

            // wait_ms(200);

            break;

        case MESSAGE:

            if (xQueueReceive(pause_queue, &pause, 0))
            {
                
                if(pause > bit_delay)
                {
                    // one or more zero's has been received
                    // depends on how long pause is
                    // paus / bit_dely = number_of_zero's
                    // it's also always followd by 1
                }
                else if(pause = 0)
                {
                    // 1
                }

                // we need a way to determine the end of the byte!

                // we also need a way to determine that the massage has been ended, maybe a long pause?

                // at last we need to send the bytes to the main

            }

            state = IDLE;

            break;
        }
    }
}

} // namespace rec