#pragma once

#pragma once

#include "boost/pfr.hpp"

#include <tuple>
#include <sstream>

#include "utils/Utils.h"
#include "serializable/Serializable.h"

namespace serializer
{

class QtSqlInsert
{
    using OutSerialized = std::string;

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        std::stringstream fields;
        std::stringstream values;
        bool first = true;

        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);

        auto processor = [&first, &fields, &values](const auto& inField){
            if (!first)
            {
                fields << ",";
                values << ",";
            }
            fields << "'" << inField.name.GetString() << "'";
            values << "'" << inField.value << "'";
            first = false;
        };


        std::apply([processor](auto&&... args) {
            (processor(args),...);
        }, tmpTuple);

        outSerialized = "INSERT INTO %1 (" + fields.str() + ") VALUES (" + values.str() + ");";
    }
};

}
