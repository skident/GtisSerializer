#pragma once

#include "boost/pfr.hpp"

#include <tuple>

#include <QJsonObject>
#include <QVariant>

#include "utils/Utils.h"
#include "serializable/Serializable.h"

namespace serializer
{

class QtJSON
{
    using OutSerialized = QJsonObject;
    using InSerialized = QJsonObject;

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);
        std::apply([&outSerialized](auto&&... args) {
            (toJsonValue(outSerialized, args),...);
        }, tmpTuple);
    }

    template <class OutDeserialized>
    static void deserialize(const InSerialized& inSerialized, OutDeserialized& outDeserialized)
    {
        auto tmpTupple = boost::pfr::structure_to_tuple(outDeserialized);
        std::apply([inSerialized](auto&&... args) {
            (fromJsonValue(inSerialized[args.name.GetString()], args),...);
        }, tmpTupple);

        outDeserialized = utils::make_struct<OutDeserialized>(tmpTupple);
    }

private:
    template <class T, class U>
    static void fromJsonValue(const QJsonValue& inData, TSerializable<T, U>& outField)
    {
        if constexpr (std::is_same<T, std::string>::value)
                outField = inData.toString().toStdString();
        else
                outField = inData.toVariant().value<T>();
    }

    template <class T, class U>
    static void toJsonValue(QJsonObject& outObject, const TSerializable<T, U>& inField)
    {
        QJsonValue val;
        if constexpr (std::is_same<decltype (inField.value), std::string>::value)
                val = inField.value.c_str();
        else if constexpr (std::is_same<decltype (inField.value), unsigned int>::value)
                val = static_cast<int>(inField);
        else
                val = inField.value;

        outObject[inField.name.GetString()] = val;
    }
};

}
