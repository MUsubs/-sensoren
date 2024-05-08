#include <Arduino.h>

class Receiver {

    enum state_t
    {
        IDLE,
        SIGNAL,
        MESSAGE
    };

private:

    state_t state = IDLE;
    int photPin = 36;
    int delayTime = 50;
    int ambientThreshold; 
    int lowThreshold;
    int highThreshold;
    


public:

    Receiver(){};
    void setup();
    void loop();
    void pauseDetected(int pause);
    void calibrate();
    bool createMessage(uint16_t &message, int &pause);
    bool isValidCheckSum(const uint16_t &message);

};