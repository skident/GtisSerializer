#pragma once

#include "boost/pfr.hpp"
#include <tuple>
#include <set>
#include <map>
#include <vector>
#include <list>

#include "nlohmann/json.hpp"
#include "meta/tuple_to_struct.h"
#include "meta/meta_property.h"

namespace eos::meta
{

////////////////////////////////////////////////
////////////////// DESERIALIZER ////////////////
////////////////////////////////////////////////
class json_deserializer
{
    using in_serialized_t = nlohmann::json;

public:
    template <class OutDeserialized>
    static void deserialize(const in_serialized_t& inSerialized, OutDeserialized& outDeserialized)
    {
        auto tmpTupple = boost::pfr::structure_to_tuple(outDeserialized);
        std::apply([inSerialized](auto&&... elem) {
            (deserialize_impl(inSerialized, elem),...);
        }, tmpTupple);

        outDeserialized = meta::make_struct<OutDeserialized>(tmpTupple);
    }

private:
    template <class T>
    static constexpr void deserialize_impl(const in_serialized_t& inSerialized,
                                           T& outValue)
    {
        if constexpr (meta::is_meta<T>::property)
        {
            auto value = inSerialized[outValue.meta_property_name()];

            if constexpr (meta::is_meta<decltype(std::declval<T>().value)>::object)
            {
                deserialize(inSerialized[outValue.meta_property_name()], outValue.value); // recursion
            }
            else
            {
                deserialize_key_value(value, outValue.value);
            }
        }
    }

    template <class T, class U>
    static void deserialize_key_value(const in_serialized_t& inSerialized,
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
    static constexpr void deserialize_key_value(const in_serialized_t& inSerialized,
                                                T& outValue)
    {
        if constexpr (meta::is_meta<T>::object)
        {
            deserialize(inSerialized, outValue);
        }
        else
        {
            outValue = inSerialized.get<T>();
        }
    }

    template <class T>
    static constexpr void deserialize_key_value(const in_serialized_t& inSerialized,
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
    static constexpr void deserialize_key_value(const in_serialized_t& inSerialized,
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
class json_serializer
{
    using out_serialized_t = nlohmann::json;

private:
    template <class T>
    static void serialize_array_value(out_serialized_t& outSerialized,
                                      const T& inValue)
    {
        if constexpr (meta::is_meta<T>::object)
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
    static constexpr void serialize_key_value(out_serialized_t& outSerialized,
                                              const std::string& inName,
                                              const T& inValue)
    {
        outSerialized[inName] = inValue;
    }

    template <class T>
    static constexpr void serialize_key_value(out_serialized_t& outSerialized,
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
    static constexpr void serialize_key_value(out_serialized_t& outSerialized,
                                              const std::string& inName,
                                              const std::set<T>& inSerializable)
    {
        serialize_key_value(outSerialized, inName, std::vector<T>(inSerializable.begin(), inSerializable.end()));
    }

    template <class T>
    static constexpr void serialize_key_value(out_serialized_t& outSerialized,
                                              const std::string& inName,
                                              const std::list<T>& inSerializable)
    {
        serialize_key_value(outSerialized, inName, std::vector<T>(inSerializable.begin(), inSerializable.end()));
    }

    template <class T, class U>
    static constexpr void serialize_key_value(out_serialized_t& outSerialized,
                                              const std::string& inName,
                                              const std::map<T, U>& inSerializable)
    {
        for (const auto& elem : inSerializable)
        {
            serialize_key_value(outSerialized[inName], elem.first, elem.second);
        }
    }

    template <class T>
    static constexpr void serialize_impl(out_serialized_t& outSerialized,
                                         const T& inSerializable)
    {
        if constexpr (meta::is_meta<T>::property)
        {
            if constexpr (meta::is_meta<decltype(std::declval<T>().value)>::object)
            {
                serialize(outSerialized[inSerializable.meta_property_name()], inSerializable.value); // recursion
            }
            else
            {
                serialize_key_value(outSerialized, inSerializable.meta_property_name(), inSerializable.value);
            }
        }
        else {
            // non-serializable value
        }
    }

public:
    template <class InSerializable>
    static void serialize(out_serialized_t& outSerialized, const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);
        std::apply([&outSerialized](auto&&... elem) {
            ((serialize_impl(outSerialized, elem)), ...);
        }, tmpTuple);
    }

    template <class InSerializable>
    static out_serialized_t serialize(const InSerializable& inSerializable)
    {
        out_serialized_t outSerialized;
        serialize(outSerialized, inSerializable);
        return outSerialized;
    }
};
////////////////////////////////////////////////
/////////////// END OF SERIALIZER //////////////
////////////////////////////////////////////////



class jsonizer
{
    using json_object = nlohmann::json;

public:
    template <class InSerializable>
    static void to_json(json_object& outSerialized, const InSerializable& inSerializable)
    {
        json_serializer::serialize(outSerialized, inSerializable);
    }

    template <class OutDeserialized>
    static void from_json(const json_object& inSerialized, OutDeserialized& outDeserialized)
    {
        json_deserializer::deserialize(inSerialized, outDeserialized);
    }

    template <class OutDeserialized>
    static void from_json(const std::string& inJson, OutDeserialized& outDeserialized)
    {
        json_deserializer::deserialize(nlohmann::json::parse(inJson), outDeserialized);
    }

};

}
