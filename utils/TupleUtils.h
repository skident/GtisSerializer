#pragma once

#include <functional>
#include <utility>
#include <string>
#include "SfinaeSerializableUtils.h"

namespace tuple_utils
{

template<std::size_t I = 0, typename... Tp>
static
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_loop(bool /*first*/, std::tuple<Tp...>& t, std::function<void(const std::string& inSeparator, decltype (std::get<I>(t)))> /*f*/)
{ }

template<std::size_t I = 0, typename... Tp>
static
inline typename std::enable_if<I < sizeof...(Tp), void>::type
for_loop(bool first, std::tuple<Tp...>& t, std::function<void(const std::string& inSeparator, decltype (std::get<I>(t)))> f)
{
    auto item = std::get<I>(t);
    const bool serializable = sfinae_utils::is_serializable<decltype(item)>::value;

    std::string separator;
    if (!first > 0 && serializable)
        separator = ", ";
    if (serializable)
        first = false;

    f(separator, item);
    for_loop<I+1, Tp...>(first, t, f);
}

}
