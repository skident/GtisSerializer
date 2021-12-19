#pragma once

#include <string>
#include "serializer/Json.h"

namespace eos
{
template <class T>
std::string toJson(const T& inObject) {
    nlohmann::json serialized;
    eos::serializer::Json::toJson(serialized, inObject);
    return std::move(serialized.dump());
}

template <class T>
T fromJson(const std::string& inJson)
{
    T t;
    fromJson(inJson, t);
    return std::move(t);
}

template <class T>
void fromJson(const std::string& inJson, T& outObject)
{
    eos::serializer::Json::fromJson(inJson, outObject);
}

}
