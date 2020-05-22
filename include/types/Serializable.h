#pragma once

#include <utility>
#include <string>
#include <bitset>
#include <vector>
#include <iostream>

// private
#define __BHR_STRUCT_NAME__ __bhr_serializable_struct__
#define __BHR_VAR_NAME__ __bhr_serializable__

// public
#define BHR_SERIALIZABLE_STRUCT bool __BHR_STRUCT_NAME__ ;
#define BHR_SERIALIZABLE bool __BHR_VAR_NAME__() const { return true; }

#define BHR_CTS(name) decltype(#name##_cts) // CTS - Compile Time String
#define BHR_TYPE(Type, Name) bhr::MetaProperty<Type, BHR_CTS(Name)> Name
#define BHR_TYPE_FLAGS(Type, Name, _Flags) bhr::MetaProperty<Type, BHR_CTS(Name), _Flags> Name
#define BHR_TYPE_INITED(Type, Name, Value) BHR_TYPE(Type, Name) = {Value}

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


///////////////////////////////

struct GitsVersion
{
    int major;
    int minor;
};

inline bool operator==(const GitsVersion& lhs, const GitsVersion& rhs)
{
    return lhs.major == rhs.major && lhs.minor == rhs.minor;
}
inline bool operator!=(const GitsVersion& lhs, const GitsVersion& rhs)
{
    return !(lhs == rhs);
}

inline bool operator>(const GitsVersion& lhs, const GitsVersion& rhs)
{
    if (lhs.major > rhs.major)
        return true;

    return lhs.major == rhs.major && lhs.minor > rhs.minor;
}

inline bool operator<(const GitsVersion& lhs, const GitsVersion& rhs)
{
    return (rhs > lhs);
}
inline bool operator<=(const GitsVersion& lhs, const GitsVersion& rhs)
{
    return (lhs == rhs || rhs > lhs);
}
////////////////////////////////

using Params = std::vector<std::pair<int, std::string>>;
using Flags = int;

template <class T,
          class Name,
          Flags _Flags = 0,
          int FromMajorVersion = 0,
          int FromMinorVersion = 0,
          int ToMajorVersion = 0,
          int ToMinorVersion = 0>
struct MetaProperty
{
private:
    Name __name;
    T value;
    Flags flags = _Flags;
    Params params;
    GitsVersion versionFrom = {FromMajorVersion, FromMinorVersion};
    GitsVersion versionTo =  {ToMajorVersion, ToMinorVersion};

public:
    BHR_SERIALIZABLE

    MetaProperty(const MetaProperty& rhs)
    {
        value = rhs.value;
        flags = rhs.flags;
        params = rhs.params;
        versionFrom = rhs.versionFrom;
        versionTo = rhs.versionTo;
    }

    MetaProperty& operator=(const MetaProperty& rhs)
    {
        value = rhs.value;
        flags = rhs.flags;
        params = rhs.params;
        versionFrom = rhs.versionFrom;
        versionTo = rhs.versionTo;

        return *this;
    }

    MetaProperty()
    {
    }

    MetaProperty(const T& inValue,
                 const Params& inParams = {},
                 const GitsVersion& inVersionFrom = {0,0},
                 const GitsVersion& inVersionTo = {0,0})
    {
        value = inValue;
        params = inParams;
        versionFrom = inVersionFrom;
        versionTo = inVersionTo;
    }

    Flags GetFlags() const {
        return flags;
    }

    T GetValue() const {
        return value;
    }

    Params GetParams() const {
        return params;
    }

    constexpr const char* GetName() const { return __name.value; }
    operator T&() { return value; }

    T& operator=(const T& inValue) {
        value = inValue;
        return value;
    }
};


template <class Name,
          Flags _Flags,
          int FromMajorVersion,
          int FromMinorVersion,
          int ToMajorVersion,
          int ToMinorVersion>
struct MetaProperty<std::string,
        Name,
        _Flags,
        FromMajorVersion,
        FromMinorVersion,
        ToMajorVersion,
        ToMinorVersion
        >
{
private:
    Name __name;

    std::string value;
    Flags flags = _Flags;
    Params params;
    GitsVersion versionFrom = {FromMajorVersion, FromMinorVersion};
    GitsVersion versionTo =  {ToMajorVersion, ToMinorVersion};

public:
    BHR_SERIALIZABLE

    MetaProperty() = default;
    MetaProperty(const char* inValue) : value(inValue) {}
    MetaProperty(const std::string& inValue,
                 const Params& inParams = {},
                 const GitsVersion& inVersionFrom = {0,0},
                 const GitsVersion& inVersionTo = {0,0})
    {
        value = inValue;
        params = inParams;
        versionFrom = inVersionFrom;
        versionTo = inVersionTo;
    }

    Flags GetFlags() const {
        return flags;
    }

    std::string GetValue() const {
        return value;
    }

    Params GetParams() const {
        return params;
    }

    constexpr const char* GetName() const { return __name.value; }
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
    template<typename C> static no& test(...);

public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename T>
class is_serializable : public sfinae_base
{
    template<typename C>
    static yes& test( decltype(std::declval<C>().__BHR_VAR_NAME__()) );

    template<typename C>
    static no& test(...);

public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
//    static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};

}

template <class T>
inline bool serializable(T)
{
    return bhr::sfinae_utils::is_serializable<T>::value;
}

//////////////////////////////////
////// END OF SFINAE UTILS ///////
//////////////////////////////////

