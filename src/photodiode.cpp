#include "photodiode.hpp"

namespace rec {

void Photodiode::addListener( PhotodiodeListener *listener ) {
    if ( currentNumberOfListeners < maxNumberOfListeners ) {
        PhotodiodeListenerArr[currentNumberOfListeners] = listener;
        currentNumberOfListeners++;
    }
}

Photodiode::Photodiode( int phot_pin, unsigned int frequency ) :
    phot_pin( phot_pin ), frequency( frequency ) {
    pinMode( phot_pin, INPUT );

    if ( frequency <= 0 ) {
        frequency = 1;
    }
    bit_delay = 1.f / (float)frequency;
    int read = analogRead( phot_pin );
    low_threshold = read - ( 0.1 * read );
}

void Photodiode::run() {
    double pulse_length = 0;
    PinStatus photo_signal;
    uint16_t photo_value = 0;
    unsigned long start;
    unsigned long end;
    unsigned long last;
    int read;

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
                    read = analogRead( phot_pin );
                    low_threshold = read - ( 0.2 * read );
                    Serial.printf("==INFO== Value read from photo_pin = %d | New low threshold = %d\n", read, low_threshold);
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