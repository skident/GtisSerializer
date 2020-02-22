#pragma once

#include "StdOstream.h"
#include <string>
#include <sstream>

namespace serializer
{

class StdString
{
    using OutSerialized = std::string;

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        std::stringstream result;
        StdOstream::serialize(result, inSerializable);
        outSerialized = result.str();
    }
};

}
