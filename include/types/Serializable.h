#pragma once

#include <utility>
#include <string>

#include <vector>
#include <map>
#include <unordered_map>


// Easy Object Serialization

#define INJECT_EOS_META_INFO(attr) static constexpr char eos_meta_info(){ return attr; }
#define EOS_OBJECT INJECT_EOS_META_INFO('o')

#define EOS_STRING(name) decltype(#name##_cts) // CTS - Compile Time String
#define EOS_PROPERTY(Type, Name) eos::Property<Type, EOS_STRING(Name)> Name

//////////////////////////////////
////////// SFINAE UTILS //////////
//////////////////////////////////
namespace eos::sfinae_utils
{

typedef char yes;
typedef char no[2];

template <typename T>
class serializable
{
    template<typename C> static yes& has_property_attr( decltype(&C::eos_meta_info) ) {return C::eos_meta_info() == 'p';}
    template<typename C> static no& has_property_attr(...) {};

    template<typename C> static yes& has_object_attr( decltype(&C::eos_meta_info) ) { return C::eos_meta_info() == 'o'; }
    template<typename C> static no& has_object_attr(...) {};

public:
    static bool const is_property = sizeof(has_property_attr<T>(0)) == sizeof(yes);
    static bool const is_object = sizeof(has_object_attr<T>(0)) == sizeof(yes);
};
}

//////////////////////////////////
////// END OF SFINAE UTILS ///////
//////////////////////////////////



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


namespace eos
{

#define INJECT_PROPERTY_BODY(Type)                                  \
    private:                                                        \
        Name mName;                                                 \
    public:                                                         \
    Type value;                                                     \
    PropertyFlags flags;                                            \
    INJECT_EOS_META_INFO('p')                                       \
                                                                    \
    Property() = default;                                           \
    Property(const Type& inValue,                                   \
                const PropertyFlags& inFlags = {})                  \
                    : value(inValue), flags(inFlags) {}             \
                                                                    \
    constexpr const char* GetName() const { return mName.value; }   \
    operator Type&() { return value; }                              \
                                                                    \
    Type& operator=(const Type& inValue) {                          \
        value = inValue;                                            \
        return value;                                               \
    }                                                               \
    Type* operator->() { return &value; }                           \


using PropertyFlags = std::unordered_map<std::string, int>;

template <class T, class Name>
struct Property
{
    INJECT_PROPERTY_BODY(T)
};

template <class Name>
struct Property<std::string, Name>
{
    using Type = std::string;
    INJECT_PROPERTY_BODY(Type);

    Property(const char* inValue) : value(inValue) {}
    operator const char* () { return value.c_str(); }
    Type& operator=(const char* inValue) {
        value = inValue;
        return value;
    }
};


template <class T, class Name>
struct Property<std::vector<T>, Name>
{
    using Type = std::vector<T>;
    INJECT_PROPERTY_BODY(Type);

    T& operator[](size_t i) {
        return value[i];
    }
};

template <class K, class V, class Name>
struct Property<std::map<K, V>, Name>
{
    using Type = std::map<K, V>;
    INJECT_PROPERTY_BODY(Type);

    V& operator[](K key) {
        return value[key];
    }
};

}
