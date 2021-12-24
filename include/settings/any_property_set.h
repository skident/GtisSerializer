#pragma once

#include "any.h"
#include <unordered_map>

namespace eos {

class any_property_set
{
private:
    using properties = std::unordered_map<std::string, eos::any>;
    properties mProperties;

public:
    virtual void propertyChanged(const std::string& inKey) { (void)inKey; }

public:
    template <class T>
    void set(const std::string& inKey, const T& inValue)
    {
        eos::any any = inValue;
        mProperties[inKey] = any;
    }

    template <class T>
    T get(const std::string& inKey, const T& inDefaultValue)
    {
        auto it = mProperties.find(inKey);
        if (it != mProperties.end())
            return it->second.get<T>(inDefaultValue);

        return inDefaultValue;
    }

    properties::iterator begin()
    {
        return mProperties.begin();
    }

    properties::iterator end()
    {
        return mProperties.end();
    }

    properties::const_iterator cbegin()
    {
        return mProperties.cbegin();
    }

    properties::const_iterator cend()
    {
        return mProperties.cend();
    }
};

}
