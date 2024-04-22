#include <Arduino.h>

class Receiver {

    enum state_t
    {
        IDLE,
        SIGNAL
    };

private:

    state_t state = IDLE;
    int photPin = 36;
    int delayTime = 50;
    int lowThreshold;
    


public:

    Receiver(){};
    void setup();
    void loop();


};