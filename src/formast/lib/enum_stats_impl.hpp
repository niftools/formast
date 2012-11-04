// for friend classes: their cpp files can use this header to access
// implementation

#ifndef FORMAST_ENUM_STATS_IMPL_HPP_INCLUDED
#define FORMAST_ENUM_STATS_IMPL_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "formast.hpp"

namespace formast
{
namespace detail
{

typedef boost::make_recursive_variant<formast::EnumConst>::type EnumStatDecl;

}
}

class formast::EnumStats::Impl : public std::vector<formast::detail::EnumStatDecl>
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    Impl(Impl const &);
    Impl & operator=(Impl const &);
public:
    Impl();
};

#endif
