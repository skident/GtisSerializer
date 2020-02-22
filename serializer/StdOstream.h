#pragma once

#include "boost/pfr.hpp"
#include <tuple>
#include <iostream>

#include "utils/Utils.h"

namespace serializer
{

class StdOstream
{
    using OutSerialized = std::ostream;
    using InSerialized = std::istream;

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);
        std::apply([&outSerialized](auto&&... elem) {
            ((outSerialized << elem.name << " : " << elem.value << ";"), ...);
        }, tmpTuple);
        outSerialized << std::endl;
    }

    template <class OutDeserialized>
    static void deserialize(const InSerialized& /*inSerialized*/, OutDeserialized& /*outDeserialized*/)
    {
        assert(false);
    }
};

}
