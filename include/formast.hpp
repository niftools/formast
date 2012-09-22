#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "formast/detail/ast.hpp"

//! Namespace for all public declarations.
namespace formast
{

//! Expression node of the abstract syntax tree.
typedef formast::detail::ast::Expr Expr;

//! Abstract base class for parsers.
class Parser
{
public:
    Parser();
    //! Parse input stream into an abstract syntax tree.
    virtual Expr parse_stream(std::istream & is) = 0;
    //! Parse string into an abstract syntax tree.
    Expr parse_string(std::string const & s);
};

//! Parser for the niftools xml format.
class XmlParser : public Parser
{
public:
    XmlParser();
    virtual Expr parse_stream(std::istream & is);
};

//! Visitor for the abstract syntax tree.
class Visitor
{
public:
    Visitor();
    virtual ~Visitor();
    virtual void expr(Expr const & e);
    virtual void expr_uint(boost::uint64_t const & n);
    virtual void expr_id(std::string const & i);
    virtual void expr_pos(Expr const & right);
    virtual void expr_neg(Expr const & right);
    virtual void expr_add(Expr const & left, Expr const & right);
    virtual void expr_sub(Expr const & left, Expr const & right);
    virtual void expr_mul(Expr const & left, Expr const & right);
    virtual void expr_div(Expr const & left, Expr const & right);
private:
    // pimpl idiom
    class ExprVisitor;
    boost::shared_ptr<ExprVisitor> _expr_visitor;
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

Simply derive a subclass from formast::Visitor
and override the desired methods.

\section Expressions

For the time being, FormAST exposes just a single node type,
namely formast::Expr,
for expressions.

Expressions are created from xml code,
via formast::parse_xml().

*/

#endif
