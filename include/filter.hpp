#ifndef R2D2_FILTER_HPP
#define R2D2_FILTER_HPP

#include "Arduino.h"

namespace sen {

class Filter {
public:
    Filter( int input_pin, int output_pin );
    void run();

private:
    int input_pin;
    int output_pin;

    enum class state_t{ INPUT, OUTPUT };
    state_t state = state_t::INPUT;
};

}  // namespace sen
#endif  // R2D2_FILTER_HPP