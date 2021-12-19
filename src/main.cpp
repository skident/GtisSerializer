#include "Example_1.h"
#include "Example_2.h"
#include "Example_3.h"

using namespace eos;

int main()
{
    example::Example_1::run();
    example2::Example_2::run();
    example3::Example_3().run();

    return 0;
}
