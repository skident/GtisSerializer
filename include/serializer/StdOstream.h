#pragma once

#include "boost/pfr.hpp"
#include <tuple>
#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "utils/Utils.h"

#include "types/Serializable.h"

namespace eos::serializer
{

class StdOstream
{
    using OutSerialized = std::ostream;
    using InSerialized = std::istream;

    static constexpr auto S_QUOTE = "\"";
    static constexpr auto S_COMMA = ",";
    static constexpr auto S_SPACE = " ";
    static constexpr auto S_COLON = ":";
    static constexpr auto S_CURLY_BRACE_OPEN = "{";
    static constexpr auto S_CURLY_BRACE_CLOSE = "}";
    static constexpr auto S_BRACKET_OPEN = "[";
    static constexpr auto S_BRACKET_CLOSE = "]";


private:
    // helper function which adds as much separators as needed into OutSerialized object
    template <class... SEPARATORS>
    static bool addSeparator(bool inFirstItem, OutSerialized& outSerialized, const SEPARATORS... inSeparators)
    {
        if (!inFirstItem)
            ((outSerialized << inSeparators),...);
        return false;
    }

    template <class T>
    static constexpr void serialize_value(OutSerialized& outSerialized,
                                         const T& inValue)
    {
        if constexpr (sfinae_utils::is_serializable_struct<T>::value)
        {
            outSerialized << S_CURLY_BRACE_OPEN;
            serialize(outSerialized, inValue); // recursion
            outSerialized << S_CURLY_BRACE_CLOSE;
        }
        else
        {
            outSerialized << S_QUOTE << inValue << S_QUOTE;
        }
    }

    template <class T>
    static constexpr void serialize_simple(OutSerialized& outSerialized,
                                           const T& inSerializable)
    {
        outSerialized << S_QUOTE << inSerializable << S_QUOTE;
    }

    template <class T>
    static constexpr void serialize_simple(OutSerialized& outSerialized,
                                           const std::vector<T>& inSerializable)
    {
        outSerialized << S_BRACKET_OPEN;
        bool first = true;
        for (const auto& elem : inSerializable)
        {
            first = addSeparator(first, outSerialized, S_COMMA, S_SPACE);
            serialize_value(outSerialized, elem);
        }
        outSerialized << S_BRACKET_CLOSE;
    }

    template <class T>
    static constexpr void serialize_simple(OutSerialized& outSerialized,
                                           const std::set<T>& inSerializable)
    {
        serialize_simple(outSerialized, std::vector<T>(inSerializable.begin(), inSerializable.end()));
    }

    template <class T, class U>
    static constexpr void serialize_simple(OutSerialized& outSerialized,
                                           const std::map<T, U>& inSerializable)
    {
        outSerialized << S_CURLY_BRACE_OPEN;
        bool first = true;
        for (const auto& elem : inSerializable)
        {
            first = addSeparator(first, outSerialized, S_COMMA, S_SPACE);
            serialize_value(outSerialized, elem.first);
            outSerialized << S_COLON << S_SPACE;
            serialize_value(outSerialized, elem.second);
        }
        outSerialized << S_CURLY_BRACE_CLOSE;
    }

    template <class T>
    static constexpr void serialize_impl(OutSerialized& outSerialized,
                                         const T& inSerializable)
    {
        if constexpr (sfinae_utils::is_serializable<T>::value)
        {
            outSerialized << S_QUOTE << inSerializable.GetName() << S_QUOTE << S_COLON << S_SPACE;
            if constexpr (sfinae_utils::is_serializable_struct<decltype(std::declval<T>().value)>::value)
            {
                outSerialized << S_CURLY_BRACE_OPEN;
                serialize(outSerialized, inSerializable.value); // recursion
                outSerialized << S_CURLY_BRACE_CLOSE;
            }
            else
            {
                serialize_simple(outSerialized, inSerializable.value);
            }
        }
    }

private:
    // For loop for tuples
    template<std::size_t I = 0, typename... Tp>
    static
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_loop(bool /*first*/, OutSerialized& /*outSerialized*/, std::tuple<Tp...>& /*t*/)
    { }

    // For loop for tuples
    template<std::size_t I = 0, typename... Tp>
    static
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    for_loop(bool first, OutSerialized& outSerialized, std::tuple<Tp...>& t)
    {
        auto item = std::get<I>(t);
        constexpr bool serializable = sfinae_utils::is_serializable<decltype(item)>::value;

        if (serializable)
        {
            first = addSeparator(first, outSerialized, S_COMMA, S_SPACE);
            serialize_impl(outSerialized, item);
        }

        for_loop<I+1, Tp...>(first, outSerialized, t);
    }


public:
    template <class InSerializable>
    static constexpr void serialize(OutSerialized& outSerialized,
                                    const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);
        for_loop(true, outSerialized, tmpTuple);
    }
};

}
