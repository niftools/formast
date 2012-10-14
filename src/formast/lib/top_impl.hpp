// for friend classes: their cpp files can use this header to access
// implementation

#ifndef FORMAST_TOP_IMPL_HPP_INCLUDED
#define FORMAST_TOP_IMPL_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "formast.hpp"

namespace formast
{
namespace detail
{

typedef boost::make_recursive_variant<formast::Class>::type TopDecl;

}
}

class formast::Top::Impl : public std::vector<formast::detail::TopDecl>
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    Impl(Impl const &);
    Impl & operator=(Impl const &);
public:
    Impl();
};

#endif
