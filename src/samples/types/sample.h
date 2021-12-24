#include <iostream>
#include "types/auto_callback.h"

namespace eos::samples::types {

using namespace eos;



void autoCallbackTest()
{
    auto lmb = [](bool b, double d) -> int {
        std::cout << "Clb called:" << b << " : " << d << std::endl;
        return 0;
    };

    auto_callback<int, bool, double> safeCallback(std::move(lmb), false, 69.);
    //    safeCallback(true, 3.14);

    auto tmp = safeCallback;
    //    tmp(false, 9.22);
}


int run()
{
    autoCallbackTest();

    return 0;
}

}
