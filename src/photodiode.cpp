#include "photodiode.hpp"

namespace rec {

void Photodiode::addListener( PhotodiodeListener *listener ) {
    if ( currentNumberOfListeners < maxNumberOfListeners ) {
        PhotodiodeListenerArr[currentNumberOfListeners] = listener;
        currentNumberOfListeners++;
    }
}

Photodiode::Photodiode( int phot_pin, unsigned int frequency ) :
    phot_pin( phot_pin ), frequency( frequency ), this_task_handle{} {
    pinMode( phot_pin, INPUT );

    if ( frequency <= 0 ) {
        Serial.printf(
            "== ERROR == frequency <= 0, setting it to 1 and continuing" );
        frequency = 1;
    }
    bit_delay = 1.f / (float)frequency;
}

void Photodiode::main() {
    double pulse_length = 0;
    PinStatus photo_signal;
    unsigned long start;
    unsigned long end;

    for ( ;; ) {
        switch ( state ) {
            case IDLE:

                start = micros();
                end = start + main_clock_us;

                while ( micros() <= end ) {
                    ;
                }

                photo_signal = digitalRead( phot_pin );

                if ( photo_signal ) {
                    state = SIGNAL;
                }

                break;

            case SIGNAL:

                start = micros();
                end = start + main_clock_us;

                while ( micros() <= end ) {
                    ;
                }

                photo_signal = digitalRead( phot_pin );

                if ( photo_signal ) {
                    pulse_length += 1 / 1000;
                } else {
                    for ( auto &listener : PhotodiodeListenerArr ) {
                        listener->pulseDetected( pulse_length );
                    }

                    pulse_length = 0;
                    state = IDLE;
                }

                break;
        }
    }
}

}  // namespace rec