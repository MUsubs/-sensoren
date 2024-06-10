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

    low_threshold = analogRead( phot_pin )-100;
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
        // Serial.printf("signal = %d\n", photo_value);
        switch ( state ) {
            
            case IDLE:

                // start = micros();
                last = micros();
                // end = start + main_clock_us;

                // while ( micros() <= end ) {
                //     // Serial.printf("Waiting IDLE reset clock = %lf\n", reset_clock);
                // }


                // photo_signal = digitalRead( phot_pin );

                // if ( photo_signal ) {
                //     state = SIGNAL;
                // }

                photo_value = analogRead( phot_pin );
                // Serial.printf( "low_threshold : %d\n", low_threshold );
                // Serial.printf( "photo_value : %d\n", photo_value );

                if ( photo_value < low_threshold ) {
                    state = SIGNAL;
                    // Serial.println("STATE = SIGNAL");
                }

                // Serial.printf("Reset clock in Photodiode = %lf\n", reset_clock);
                reset_clock += (micros() - last) / 1'000'000.0;
                if(reset_clock >= 2 || digitalRead(3)){
                    for (auto &listener : PhotodiodeListenerArr) {
                        listener->resetQueue();
                    }
                    reset_clock = 0;
                }

                break;

            case SIGNAL:
                start = micros();

                // photo_signal = digitalRead( phot_pin );

                // if ( photo_signal ) {
                //     pulse_length += 1 / 1000;
                // } else {
                //     for ( auto &listener : PhotodiodeListenerArr ) {
                //         listener->pulseDetected( pulse_length );
                //     }

                //     pulse_length = 0;
                //     state = IDLE;
                // }

                photo_value = analogRead( phot_pin );
                // Serial.printf( "low_threshold : %d\n", low_threshold );
                // Serial.printf( "photo_value : %d\n", photo_value );

                if ( photo_value < low_threshold ) {
                    pulse_length += (micros() - start) / 1'000'000.0;
                } else {
                    // Serial.printf("Pulse length sent = %lf\n", pulse_length);
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