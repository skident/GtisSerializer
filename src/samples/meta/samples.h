#pragma once

#include "sample_1.h"
#include "sample_2.h"
#include "sample_3.h"
#include "sample_4.h"

namespace eos::samples::meta {

int run()
{
    Example_1::run();
    Example_2::run();
    Example_3().run();
    Example_4::run();

    return 0;
}

}
