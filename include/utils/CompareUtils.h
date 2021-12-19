#pragma once

#include "serializer/TSerializer.h"
#include "serializer/StdString.h"
#include <string>
#include <utility>

namespace eos::utils
{

template <class T>
bool AreEquals(const T& lhs, const T& rhs)
{
    using namespace serializer;
    std::string lhs_str;
    std::string rhs_str;
    Serializer<StdString>::serialize(lhs_str, lhs);
    Serializer<StdString>::serialize(rhs_str, rhs);

    return (std::hash<std::string>{}(lhs_str) == std::hash<std::string>{}(rhs_str));
}

}
