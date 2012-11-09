// for friend classes: their cpp files can use this header to access
// implementation

#ifndef FORMAST_STATS_IMPL_HPP_INCLUDED
#define FORMAST_STATS_IMPL_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "formast.hpp"

namespace formast
{
namespace detail
{

typedef boost::make_recursive_variant<formast::Field, formast::If>::type StatDecl;

}
}

class formast::Stats::Impl : public std::vector<formast::detail::StatDecl>
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    Impl(Impl const &);
    Impl & operator=(Impl const &);
public:
    Impl();
};

#endif
