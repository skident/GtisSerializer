#pragma once

#include <utility>
#include <iostream>

namespace sfinae_utils
{

struct sfinae_base {
  typedef char yes[1];
  typedef char no[2];
};


template <typename T>
class is_serializable_struct : public sfinae_base
{
    template<typename C> static yes& test( decltype(std::declval<C>().__gits_serializable_struct__) );
    template<typename C> static no& test(...) {};

public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};

//template <typename T, typename U,template <typename, typename> class K>
template <typename T>
class is_serializable : public sfinae_base
{
    template<typename C> static yes& test( decltype(std::declval<C>().__gits_serializable__) );
    template<typename C> static no& test(...) {};

public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};


template<typename T>
class has_ostream_operator : sfinae_base
{
private:
  // this may quietly fail:
  template<typename U> static yes& test(
      size_t (*n)[ sizeof( std::cout << * static_cast<U*>(0) ) ] );

  // "..." provides fallback in case above fails
  template<typename U> static no& test(...);

public:
  static bool const value = sizeof( test<T>( NULL ) ) == sizeof( yes );
};

}
