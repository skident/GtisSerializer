#include "samples/meta/samples.h"
#include "samples/settings/sample.h"
#include "samples/profile/sample.h"
#include "samples/types/sample.h"

using namespace eos;

int main()
{
    samples::meta::run();
    samples::settings::run();
    samples::profile::run();
    samples::types::run();

    return 0;
}
