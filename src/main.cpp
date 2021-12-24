#include "example_1.h"
#include "example_2.h"
#include "example_3.h"
#include "example_4.h"

using namespace eos;

int main()
{
    Example_1::run();
    Example_2::run();
    Example_3().run();
    Example_4::run();

    return 0;
}
