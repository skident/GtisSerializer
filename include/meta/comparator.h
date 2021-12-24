#pragma once

#include "stringifier.h"
#include <string>
#include <utility>

namespace eos::meta
{

template <class T>
bool are_equals(const T& lhs, const T& rhs)
{
    using namespace meta;
    std::string lhs_str;
    std::string rhs_str;
    stringifier::to_string(lhs_str, lhs);
    stringifier::to_string(rhs_str, rhs);

    return (std::hash<std::string>{}(lhs_str) == std::hash<std::string>{}(rhs_str));
}

}
