#include "vlc-receiver.hpp"


Receiver::Receiver()
{
    
}

void Receiver::pauseDetected(int pause)
{
    // pausesChannel.write(pause);
}

bool Receiver::createMessage(uint16_t &message, int &pause)
{

    for (unsigned int i = 0; i < 16; i++)
    {
        // pause = pausesChannel.read();

        if (pause > 200 && pause < 2000)
        {
            message |= (((pause > 1000) ? 0 : 1) << i);
        }

        else if (pause > 3500 && pause < 5000)
        {
            return false;
        }
    }
    return true;
}

bool Receiver::isValidCheckSum(const uint16_t &message)
{
    unsigned int checkBitOne = 1;
    unsigned int checkBitTwo = 6;
    unsigned int checkBitThree = 11;

    for (unsigned int i = 0; i < 5; i++)
    {
        if (((message >> checkBitThree) & 1) == (((message >> checkBitOne) & 1) ^ ((message >> checkBitTwo) & 1)))
        {
            checkBitThree++;
            checkBitOne++;
            checkBitTwo++;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void Receiver::loop()
{
    int pause;
    uint16_t firstMessage;
    uint16_t secondMessage;
    bool messageResult;

    for (;;)
    {
        switch (state)
        {
        case IDLE:

            // wait(pausesChannel);

            // pause = pausesChannel.read();

            if (pause > 3500 && pause < 5000)
            {
                state = MESSAGE;
            }

            // wait_ms(200);

            break;

        case MESSAGE:
            firstMessage = 0x0;
            secondMessage = 0x0;

            messageResult = createMessage(firstMessage, pause);
            // logger.logInt(firstMessage);

            // check first message
            if (!messageResult)
            {
                state = IDLE;
                break;
            }

            // check checksum
            if (!isValidCheckSum(firstMessage))
            {
                state = IDLE;
                break;
            }

            // check pausebit
            // pause = pausesChannel.read();

            if (!(pause > 2500 && pause < 3500))
            {
                state = IDLE;
                break;
            }

            // check second message
            messageResult = createMessage(secondMessage, pause);
            if (!messageResult)
            {
                state = IDLE;
                break;
            }

            if (firstMessage == secondMessage)
            {
                // receiveIRController.sendMessage(firstMessage);
            }

            state = IDLE;

            break;
        }
    }
}
