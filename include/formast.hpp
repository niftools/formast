#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <string>

//! Namespace for all public declarations.
namespace formast
{

//! Expression node of the abstract syntax tree.
class Expr
{
public:
    template<typename Iterator>
    bool parse_xml(Iterator & iter, Iterator end) {
        // TODO implement
        return false;
    }
};

//! Visitor for the abstract syntax tree.
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

/*! \mainpage FormAST Documentation

FormAST stands for *file format abstract syntax tree*.
It's primary purpose is to expose the abstract syntax tree
of a file format description.

\section Visitation

Rather than exposing the data structure
representing the abstract syntax tree directly,
FormAST exposes the structure by means of
<a href="http://en.wikipedia.org/wiki/Visitor_pattern">visitation</a>.
In practice, visitation provides every means for evaluating
the abstract syntax tree,
or for compiling it into another tree structure
for whatever sofisticated purpose.
Moreover, the visitor model allows new language features to be added
with minimal change to the public API.

Simply derive a subclass from formast::Visit
and override the desired methods.

\section Expressions

For the time being, FormAST exposes just a single node type,
namely formast::Expr,
for expressions.

Expressions are created from xml code,
via formast::Expr::parse_xml().

*/

#endif
