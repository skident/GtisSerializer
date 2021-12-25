#include "samples/meta/samples.h"
#include "samples/settings/sample.h"
#include "samples/profile/sample.h"
#include "samples/types/sample.h"
#include "samples/logger/sample.h"

int main()
{
    using namespace eos;

    samples::meta::run();
    samples::settings::run();
    samples::profile::run();
    samples::types::run();
    samples::logger::run();

    return 0;
}
