#pragma once

#pragma once

#include "boost/pfr.hpp"

#include <tuple>
#include <sstream>

#include "utils/Utils.h"
#include "serializable/Serializable.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

namespace serializer
{

class QtSqlSelect
{
    using InSerialized = QSqlQuery;

public:
    template <class OutDeserialized>
    static void deserialize(const InSerialized& inSerialized, OutDeserialized& outDeserialized)
    {
        auto rec = inSerialized.record();
        auto tmpTupple = boost::pfr::structure_to_tuple(outDeserialized);

        auto fromValue = [&rec](const auto& inData, auto& outField) {
            QVariant res = inData.value(rec.indexOf(outField.name.GetString()));
            if constexpr (std::is_same<decltype (outField.value), std::string>::value)
                outField = res.toString().toStdString();
            else
                outField = res.value<decltype (outField.value)>();
        };

        std::apply([inSerialized, fromValue](auto&&... elem) {
            (fromValue(inSerialized, elem),...);
        }, tmpTupple);

        outDeserialized = utils::make_struct<OutDeserialized>(tmpTupple);
    }

    template <class OutDeserialized>
    static void deserialize_all(InSerialized& inSerialized, std::list<OutDeserialized>& outDeserialized)
    {
        while (inSerialized.next()) {
            OutDeserialized tmp;
            deserialize(inSerialized, tmp);
            outDeserialized.push_back(tmp);
        }
    }
};

}
