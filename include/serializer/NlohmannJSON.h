#pragma once

#include "boost/pfr.hpp"
#include <tuple>
#include <set>
#include <map>
#include <vector>
#include <list>

#include "nlohmann/json.hpp"
#include "utils/Utils.h"
#include "types/Serializable.h"

namespace bhr::serializer
{

////////////////////////////////////////////////
////////////////// DESERIALIZER ////////////////
////////////////////////////////////////////////
class NlohmannDeserializer
{
    using InSerialized = nlohmann::json;

public:
    template <class OutDeserialized>
    static void deserialize(const InSerialized& inSerialized, OutDeserialized& outDeserialized)
    {
        auto tmpTupple = boost::pfr::structure_to_tuple(outDeserialized);
        std::apply([inSerialized](auto&&... elem) {
            (deserialize_impl(inSerialized, elem),...);
        }, tmpTupple);

        outDeserialized = utils::make_struct<OutDeserialized>(tmpTupple);
    }

private:
    template <class T>
    static constexpr void deserialize_impl(const InSerialized& inSerialized,
                                           T& outValue)
    {
        if constexpr (sfinae_utils::is_serializable<T>::value)
        {
            auto value = inSerialized[outValue.GetName()];

            if constexpr (sfinae_utils::is_serializable_struct<decltype(std::declval<T>().value)>::value)
            {
                deserialize(inSerialized[outValue.GetName()], outValue.value); // recursion
            }
            else
            {
                deserialize_key_value(value, outValue.value);
            }
        }
    }

    template <class T, class U>
    static void deserialize_key_value(const InSerialized& inSerialized,
                                      std::map<T, U>& outMap)
    {
        for (const auto& [key, value] : inSerialized.items())
        {
            T t;
            U u;
            deserialize_key_value(key, t);
            deserialize_key_value(value, u);
            outMap.insert({t, u});
        }
    }


    template <class T>
    static constexpr void deserialize_key_value(const InSerialized& inSerialized,
                                                T& outValue)
    {
        if constexpr (sfinae_utils::is_serializable_struct<T>::value)
        {
            deserialize(inSerialized, outValue);
        }
        else
        {
            outValue = inSerialized.get<T>();
        }
    }

    template <class T>
    static constexpr void deserialize_key_value(const InSerialized& inSerialized,
                                                std::vector<T>& outValue)
    {
        if (inSerialized.is_array())
        {
            int size = inSerialized.size();
            for (int i = 0; i < size; i++)
            {
                const auto& subValue = inSerialized.at(i);

                T tmp;
                deserialize_key_value(subValue, tmp);
                outValue.push_back(tmp);
            }
        }
    }

    template <class T>
    static constexpr void deserialize_key_value(const InSerialized& inSerialized,
                                                std::set<T>& outValue)
    {
        std::vector<T> tmp;
        deserialize_key_value(inSerialized, tmp);
        outValue = std::set<T>(tmp.begin(), tmp.end());
    }
};

////////////////////////////////////////////////
/////////////// END OF DESERIALIZER ////////////
////////////////////////////////////////////////


////////////////////////////////////////////////
/////////////////// SERIALIZER /////////////////
////////////////////////////////////////////////
class NlohmannSerializer
{
    using OutSerialized = nlohmann::json;

private:
    template <class T>
    static void serialize_array_value(OutSerialized& outSerialized,
                                      const T& inValue)
    {
        if constexpr (sfinae_utils::is_serializable_struct<T>::value)
        {
            nlohmann::json obj;
            serialize(obj, inValue); // recursion
            outSerialized.push_back(obj);
        }
        else
        {
            outSerialized.push_back(inValue);
        }
    }

    template <class T>
    static constexpr void serialize_key_value(OutSerialized& outSerialized,
                                              const std::string& inName,
                                              const T& inValue)
    {
        outSerialized[inName] = inValue;
    }

    template <class T>
    static constexpr void serialize_key_value(OutSerialized& outSerialized,
                                              const std::string& inName,
                                              const std::vector<T>& inSerializable)
    {
        outSerialized[inName] = {};
        for (const auto& elem : inSerializable)
        {
            serialize_array_value(outSerialized[inName], elem);
        }
    }

    template <class T>
    static constexpr void serialize_key_value(OutSerialized& outSerialized,
                                              const std::string& inName,
                                              const std::set<T>& inSerializable)
    {
        serialize_key_value(outSerialized, inName, std::vector<T>(inSerializable.begin(), inSerializable.end()));
    }

    template <class T>
    static constexpr void serialize_key_value(OutSerialized& outSerialized,
                                              const std::string& inName,
                                              const std::list<T>& inSerializable)
    {
        serialize_key_value(outSerialized, inName, std::vector<T>(inSerializable.begin(), inSerializable.end()));
    }

    template <class T, class U>
    static constexpr void serialize_key_value(OutSerialized& outSerialized,
                                              const std::string& inName,
                                              const std::map<T, U>& inSerializable)
    {
        for (const auto& elem : inSerializable)
        {
            serialize_key_value(outSerialized[inName], elem.first, elem.second);
        }
    }

    template <class T>
    static constexpr void serialize_impl(OutSerialized& outSerialized,
                                         const T& inSerializable)
    {
        if constexpr (sfinae_utils::is_serializable<T>::value)
        {
            if constexpr (sfinae_utils::is_serializable_struct<decltype(std::declval<T>().GetValue())>::value)
            {
                serialize(outSerialized[inSerializable.GetName()], inSerializable.GetValue()); // recursion
            }
            else
            {
                serialize_key_value(outSerialized, inSerializable.GetName(), inSerializable.GetValue());
            }
        }
        else {
            // non-serializable value
        }
    }

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);
        std::apply([&outSerialized](auto&&... elem) {
            ((serialize_impl(outSerialized, elem)), ...);
        }, tmpTuple);
    }

    template <class InSerializable>
    static OutSerialized serialize(const InSerializable& inSerializable)
    {
        OutSerialized outSerialized;
        serialize(outSerialized, inSerializable);
        return outSerialized;
    }
};
////////////////////////////////////////////////
/////////////// END OF SERIALIZER //////////////
////////////////////////////////////////////////



class NlohmannJSON
{
    using OutSerialized = nlohmann::json;
    using InSerialized = nlohmann::json;

public:
    template <class InSerializable>
    static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
    {
        NlohmannSerializer::serialize(outSerialized, inSerializable);
    }

    template <class OutDeserialized>
    static void deserialize(const InSerialized& inSerialized, OutDeserialized& outDeserialized)
    {
        NlohmannDeserializer::deserialize(inSerialized, outDeserialized);
    }
};

}
