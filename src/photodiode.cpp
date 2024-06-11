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

    if ((int)low_threshold-200 < 0) {
        Serial.println("==ERROR== WTF");
    } else {
        low_threshold = analogRead( phot_pin ) - 200;
    }
    Serial.printf( "threshold: %d\n", low_threshold );
}

void Photodiode::run() {
    double pulse_length = 0;
    PinStatus photo_signal;
    uint16_t photo_value = 0;
    unsigned long start;
    unsigned long end;
    unsigned long last;

    double reset_clock = 0;

    for ( ;; ) {
        switch ( state ) {
            case IDLE:

                last = micros();
                photo_value = analogRead( phot_pin );

                if ( photo_value < low_threshold ) {
                    state = SIGNAL;
                }

                reset_clock += ( micros() - last ) / 1'000'000.0;
                if ( reset_clock >= 2 || digitalRead( 3 ) ) {
                    for ( auto &listener : PhotodiodeListenerArr ) {
                        listener->resetQueue();
                    }
                    reset_clock = 0;
                }

                break;

            case SIGNAL:
                start = micros();

                photo_value = analogRead( phot_pin );

                if ( photo_value < low_threshold ) {
                    pulse_length += ( micros() - start ) / 1'000'000.0;
                } else {
                    pulse_length += ( micros() - start ) / 1'000'000.0;
                    for ( auto &listener : PhotodiodeListenerArr ) {
                        listener->pulseDetected( pulse_length );
                    }

                    pulse_length = 0;
                    reset_clock = 0;
                    state = IDLE;
                }

                break;
        }
    }
}

}  // namespace rec