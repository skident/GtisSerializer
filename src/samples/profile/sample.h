#include <iostream>
#include "profile/performance.h"

namespace eos::samples::profile {

using namespace eos;


void performanceTest() {
    performance performance(__PRETTY_FUNCTION__);
    ADD_PERFORMANCE_MARK(performance);
    int x = 9;
    ADD_PERFORMANCE_MARK(performance);
    x *= x;
    ADD_PERFORMANCE_MARK_ARGS(performance, " x=", x);
}

int run()
{
    performanceTest();

    return 0;
}

}
