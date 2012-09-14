#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <string>

//! Namespace for all public declarations.
namespace formast
{

class Expr
{
public:
    template<typename iterator_type>
    Expr(iterator_type iter, iterator_type end);
};


template <typename type>
class Visitor
{
public:
    typedef type result_type;
    virtual result_type expr(Expr const & expr) const;
    virtual result_type uint(boost::uint64_t const & n) const = 0;
    virtual result_type id(std::string const & i) const = 0;
    virtual result_type pos(Expr const & right) const = 0;
    virtual result_type neg(Expr const & right) const = 0;
    virtual result_type add(Expr const & left, Expr const & right) const = 0;
    virtual result_type sub(Expr const & left, Expr const & right) const = 0;
    virtual result_type mul(Expr const & left, Expr const & right) const = 0;
    virtual result_type div(Expr const & left, Expr const & right) const = 0;
};

} // namespace formast

#endif
