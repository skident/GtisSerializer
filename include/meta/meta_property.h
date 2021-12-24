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
#define EOS_PROPERTY(Type, Name) eos::meta_property<Type, EOS_STRING(Name)> Name

//////////////////////////////////
////////// SFINAE UTILS //////////
//////////////////////////////////
namespace eos::meta
{

typedef char yes;
typedef char no[2];

template <typename T>
class is_meta
{
    template<typename C> static yes& has_property_attr( decltype(&C::eos_meta_info) ) {return C::eos_meta_info() == 'p';}
    template<typename C> static no& has_property_attr(...) {};

    template<typename C> static yes& has_object_attr( decltype(&C::eos_meta_info) ) { return C::eos_meta_info() == 'o'; }
    template<typename C> static no& has_object_attr(...) {};

public:
    static bool const property = sizeof(has_property_attr<T>(0)) == sizeof(yes);
    static bool const object = sizeof(has_object_attr<T>(0)) == sizeof(yes);
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
struct compile_time_string {
    static constexpr const char value[sizeof...(Chars)+1] = {Chars...,'\0'};
    static constexpr int size = sizeof...(Chars);
};

template<char ... Chars>
constexpr const char compile_time_string<Chars...>::value[sizeof...(Chars)+1];

template<typename CharT, CharT ...String>
constexpr compile_time_string<String...> operator"" _cts()
{
    return compile_time_string<String...>();
}

//////////////////////////////////
/// END OF Compile Time String ///
//////////////////////////////////


namespace eos
{

#define INJECT_PROPERTY_BODY(type)                                              \
    private:                                                                    \
        Name mName;                                                             \
    public:                                                                     \
    type value;                                                                 \
    meta_property_flags flags;                                                  \
    INJECT_EOS_META_INFO('p')                                                   \
                                                                                \
    meta_property() = default;                                                  \
    meta_property(const type& inValue,                                          \
                const meta_property_flags& inFlags = {})                        \
                    : value(inValue), flags(inFlags) {}                         \
                                                                                \
    constexpr const char* meta_property_name() const { return mName.value; }    \
    operator type&() { return value; }                                          \
                                                                                \
    type& operator=(const type& inValue) {                                      \
        value = inValue;                                                        \
        return value;                                                           \
    }                                                                           \
    type* operator->() { return &value; }                                       \


using meta_property_flags = std::unordered_map<std::string, int>;

template <class T, class Name>
struct meta_property
{
    INJECT_PROPERTY_BODY(T)
};

template <class Name>
struct meta_property<std::string, Name>
{
    using type = std::string;
    INJECT_PROPERTY_BODY(type);

    meta_property(const char* inValue) : value(inValue) {}
    operator const char* () { return value.c_str(); }
    type& operator=(const char* inValue) {
        value = inValue;
        return value;
    }
};


template <class T, class Name>
struct meta_property<std::vector<T>, Name>
{
    using type = std::vector<T>;
    INJECT_PROPERTY_BODY(type);

    T& operator[](size_t i) {
        return value[i];
    }
};

template <class K, class V, class Name>
struct meta_property<std::map<K, V>, Name>
{
    using type = std::map<K, V>;
    INJECT_PROPERTY_BODY(type);

    V& operator[](K key) {
        return value[key];
    }
};

}
