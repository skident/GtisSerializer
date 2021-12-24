#pragma once

#include <any>
#include <sstream>
#include <iostream>

namespace eos {

enum EType {
    eType_None,
    eType_Bool,
    eType_Char,
    eType_Int32,
    eType_Int64,
    eType_Double,
    eType_String,
    eType_Custom,
};

class any
{
    EType mType = eType_None;
    std::any mValue;

private:
    template <class T, class U>
    static T try_cast(const U& inValue)
    {
        if constexpr (std::is_same<T, U>::value)
                return inValue;

        if constexpr (std::is_convertible<U, T>::value)
                return inValue;

        T value = {};
        std::stringstream ss;
        ss << inValue;
        ss >> value;

        return value;
    }

public:
    any()
    {

    }

    template<class T>
    any(const T& inValue)
    {
        set(inValue);
    }

    template<class T>
    void set(const T& inValue)
    {
        mValue = inValue;

        if constexpr (std::is_same<T, bool>::value) {
            mType = eType_Bool;
        }
        else if constexpr (std::is_same<T, char>::value) {
            mType = eType_Char;
        }
        else if constexpr (std::is_same<T, int32_t>::value) {
            mType = eType_Int32;
        }
        else if constexpr (std::is_same<T, int64_t>::value) {
            mType = eType_Int64;
        }
        else if constexpr (std::is_same<T, double>::value) {
            mType = eType_Double;
        }
        else if constexpr (std::is_same<T, std::string>::value) {
            mType = eType_String;
        }
        else {
            mType = eType_Custom;
        }
    }

    void set(const char* inValue)
    {
        set<std::string>(std::string(inValue));
    }

    template <class T>
    T get(const T& inDefaultValue = {}) const
    {
        switch (type())
        {
        case eType_None:
            return inDefaultValue;

        case eType_Bool:
            return try_cast<T>(extract<bool>());

        case eType_Char:
            return try_cast<T>(extract<char>());

        case eType_Int32:
            return try_cast<T>(extract<int32_t>());

        case eType_Int64:
            return try_cast<T>(extract<int64_t>());

        case eType_Double:
            return try_cast<T>(extract<double>());

        case eType_String:
            return try_cast<T>(extract<std::string>());

        default:
            break;
        }

        return extract<T>(inDefaultValue);
    }

    EType type() const
    {
        return mType;
    }

    template <class T>
    T extract(const T& inDefaultValue = {}) const
    {
        try
        {
            return std::any_cast<T>(mValue);
        }
        catch (std::bad_any_cast& e)
        {
            std::cerr << "BAD ANY CAST: " << e.what() << " " << mValue.type().name() << std::endl;
        }
        return inDefaultValue;
    }

};

}
