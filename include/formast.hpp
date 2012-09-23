#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "formast/detail/ast.hpp"

namespace formast
{

typedef formast::detail::ast::Expr Expr;

class Parser
{
public:
    Parser();
    virtual Expr parse_stream(std::istream & is) = 0;
    Expr parse_string(std::string const & s);
};

class XmlParser : public Parser
{
public:
    XmlParser();
    virtual Expr parse_stream(std::istream & is);
};

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

#endif
