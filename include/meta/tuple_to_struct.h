#pragma once

#include <tuple>

//https://stackoverflow.com/questions/38561242/struct-to-from-stdtuple-conversion
namespace eos::meta
{

namespace details
{

template< typename result_type, typename ...types, std::size_t ...indices >
result_type
make_struct(std::tuple< types... > t, std::index_sequence< indices... >) // &, &&, const && etc.
{
    return {std::get< indices >(t)...};
}

} // details

template< typename result_type, typename ...types >
result_type
make_struct(std::tuple< types... > t) // &, &&, const && etc.
{
    return details::make_struct< result_type, types... >(t, std::index_sequence_for< types... >{}); // if there is repeated types, then the change for using std::index_sequence_for is trivial
}

}
