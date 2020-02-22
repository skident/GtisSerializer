#pragma once

#include "boost/pfr.hpp"
#include <tuple>

#include "nlohmann/json.hpp"
#include "utils/Utils.h"
#include "serializable/Serializable.h"

namespace serializer
{

class NlohmannJSON
{
    using OutSerialized = nlohmann::json;
    using InSerialized = nlohmann::json;

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);
        std::apply([&outSerialized](auto&&... elem) {
            ((outSerialized[elem.name] = elem.value), ...);
        }, tmpTuple);
    }

    template <class OutDeserialized>
    static void deserialize(const InSerialized& inSerialized, OutDeserialized& outDeserialized)
    {
        auto tmpTupple = boost::pfr::structure_to_tuple(outDeserialized);

        auto fromJsonValue = [](const auto& inData, auto& outField) {
            if constexpr (std::is_same<decltype (outField.value), int>::value)
                    outField = inData[outField.name].template get<int>();
            else
                    outField = inData[outField.name].template get<decltype (outField.value)>();
        };

        std::apply([inSerialized, fromJsonValue](auto&&... elem) {
            (fromJsonValue(inSerialized, elem),...);
        }, tmpTupple);

        outDeserialized = utils::make_struct<OutDeserialized>(tmpTupple);
    }
};

}
