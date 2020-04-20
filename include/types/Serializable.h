#pragma once

#include <utility>
#include <string>

// private
#define __BHR_STRUCT_NAME__ __bhr_serializable_struct__
#define __BHR_VAR_NAME__ __bhr_serializable__

// public
#define BHR_SERIALIZABLE_STRUCT char __BHR_STRUCT_NAME__;
#define BHR_SERIALIZABLE char __BHR_VAR_NAME__;

#define BHR_CTS(name) decltype(#name##_cts) // CTS - Compile Time String
#define BHR_TYPE(Type, Name) bhr::MetaProperty<Type, BHR_CTS(Name)> Name
#define BHR_TYPE_INITED(Type, Name, Value) BHR_TYPE(Type, Name) = Value

//////////////////////////////////
////// Compile Time String ///////
//////////////////////////////////

// origin: https://habr.com/ru/post/243581/
template<char ... Chars>
struct CompileTimeString {
    static constexpr const char value[sizeof...(Chars)+1] = {Chars...,'\0'};
    static constexpr int size = sizeof...(Chars);
};

template<char ... Chars>
constexpr const char CompileTimeString<Chars...>::value[sizeof...(Chars)+1];

template<typename CharT, CharT ...String>
constexpr CompileTimeString<String...> operator"" _cts()
{
    return CompileTimeString<String...>();
}

//////////////////////////////////
/// END OF Compile Time String ///
//////////////////////////////////


namespace bhr
{

template <class T, class Name>
struct MetaProperty
{
private:
    Name __name;

public:
    T value;
    BHR_SERIALIZABLE

    MetaProperty() = default;
    MetaProperty(const T& inValue) : value(inValue) {}

    constexpr const char* GetName() const { return __name.value; }
    operator T&() { return value; }

    T& operator=(const T& inValue) {
        value = inValue;
        return value;
    }
};


template <class Name>
struct MetaProperty<std::string, Name>
{
private:
    Name name;

public:
    std::string value;
    BHR_SERIALIZABLE

    MetaProperty() = default;
    MetaProperty(const char* inValue) : value(inValue) {}

    constexpr const char* GetName() const { return name.value; }
    operator std::string&() { return value; }
    operator const char* () { return value.c_str(); }

    std::string& operator=(const char* inValue) {
        value = inValue;
        return value;
    }
};

}


//////////////////////////////////
////////// SFINAE UTILS //////////
//////////////////////////////////
namespace bhr::sfinae_utils
{

struct sfinae_base {
  typedef char yes[1];
  typedef char no[2];
};


template <typename T>
class is_serializable_struct : public sfinae_base
{
    template<typename C> static yes& test( decltype(std::declval<C>().__BHR_STRUCT_NAME__) );
    template<typename C> static no& test(...) {};

public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename T>
class is_serializable : public sfinae_base
{
    template<typename C> static yes& test( decltype(std::declval<C>().__BHR_VAR_NAME__) );
    template<typename C> static no& test(...) {};

public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};

}

//////////////////////////////////
////// END OF SFINAE UTILS ///////
//////////////////////////////////

