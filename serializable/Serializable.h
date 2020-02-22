#pragma once

#include <string>

//https://stackoverflow.com/questions/1826464/c-style-strings-as-template-arguments

template <char... chars>
using tstring = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr tstring<chars...> operator""_gitsstr() { return { }; }

template <typename>
struct SerializableName;

template <char... elements>
struct SerializableName<tstring<elements...>> {
    const char* GetString() const
    {
        static constexpr char str[sizeof...(elements) + 1] = { elements..., '\0' };
        return str;
    }

    operator const char*()
    {
        return GetString();
    }

    operator std::string()
    {
        return GetString();
    }
};


template <class T, class U>
struct TSerializable
{
    U name;
    T value;

    TSerializable() = default;
    TSerializable(const T& inValue) { this->value = inValue; }

    TSerializable& operator=(const T& inValue)
    {
        value = inValue;
        return  *this;
    }

    operator T() const
    {
        return value;
    }
};

#define SERIALIZABLE(Type, Name) TSerializable<Type, SerializableName<decltype("" #Name ""_gitsstr)> > Name
#define SERIALIZABLE_VAL(Type, Name, Value) TSerializable<Type, SerializableName<decltype("" #Name ""_gitsstr)> > Name = Value
