#pragma once

#include <string>
#include "meta/jsonizer.h"
#include "meta/stringifier.h"

namespace eos
{
template <class T>
std::string to_json(const T& inObject) {
    nlohmann::json serialized;
    eos::meta::jsonizer::to_json(serialized, inObject);
    return std::move(serialized.dump());
}

template <class T>
T from_json(const std::string& inJson)
{
    T t;
    from_json(inJson, t);
    return std::move(t);
}

template <class T>
void from_json(const std::string& inJson, T& outObject)
{
    eos::meta::jsonizer::from_json(inJson, outObject);
}

template <class MetaObject>
std::string to_string(const MetaObject& inObject)
{
    return eos::meta::stringifier::to_string(inObject);
}
}
