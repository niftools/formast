#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <string>

#include "formast/detail/ast.hpp"
#include "formast/detail/visitor.hpp"

//! Namespace for all public declarations.
namespace formast
{

//! Expression node of the abstract syntax tree.
typedef formast::detail::ast::Expr Expr;

template<typename Iterator>
bool parse_xml(Iterator & iter, Iterator end, Expr & e)
{
    // TODO implement
    return false;
}

//! Visitor for the abstract syntax tree.
template <typename type>
class Visitor
{
public:
    Visitor() : _impl(*this) {};
    virtual type expr(const Expr & e) const {
        return _impl.expr(e);
    };
    virtual type uint(boost::uint64_t const & n) const = 0;
    virtual type id(std::string const & i) const = 0;
    virtual type pos(Expr const & right) const = 0;
    virtual type neg(Expr const & right) const = 0;
    virtual type add(Expr const & left, Expr const & right) const = 0;
    virtual type sub(Expr const & left, Expr const & right) const = 0;
    virtual type mul(Expr const & left, Expr const & right) const = 0;
    virtual type div(Expr const & left, Expr const & right) const = 0;
private:
    formast::detail::visitor::VisitorImpl<type, Visitor<type> > _impl;
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
via formast::parse_xml().

*/

#endif
