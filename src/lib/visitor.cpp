#include <cassert>
#include <boost/variant/apply_visitor.hpp>
#include <stdexcept>

#include "formast.hpp"

class formast::Visitor::ExprVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    ExprVisitor(ExprVisitor const &);
    ExprVisitor & operator=(ExprVisitor const &);

public:
    typedef void result_type;

    ExprVisitor(Visitor & visitor) : visitor(visitor) {};

    result_type expr(formast::detail::ast::Expr const & e) {
        assert(e != 0);
        return boost::apply_visitor(*this, *e);
    }

    result_type operator()(boost::uint64_t const& n) {
        return visitor.expr_uint(n);
    }

    result_type operator()(std::string const& i) {
        return visitor.expr_id(i);
    }

    result_type operator()(const formast::detail::ast::unary_op & x) {
        switch (x.op) {
        case '-':
            return visitor.expr_neg(x.right);
        case '+':
            return visitor.expr_pos(x.right);
        default:
            throw std::runtime_error("unknown unary operator");
        }
    }

    result_type operator()(const formast::detail::ast::binary_op & x) {
        switch (x.op) {
        case '+':
            return visitor.expr_add(x.left, x.right);
        case '-':
            return visitor.expr_sub(x.left, x.right);
        case '*':
            return visitor.expr_mul(x.left, x.right);
        case '/':
            return visitor.expr_div(x.left, x.right);
        default:
            throw std::runtime_error("unknown binary operator");
        }
    }

    Visitor & visitor;

};

formast::Visitor::Visitor()
{
    _expr_visitor = boost::shared_ptr<ExprVisitor>(new ExprVisitor(*this));
}

formast::Visitor::~Visitor()
{
}

void formast::Visitor::expr(Expr const & e)
{
    _expr_visitor->expr(e);
}

void formast::Visitor::expr_uint(boost::uint64_t const & n) {};
void formast::Visitor::expr_id(std::string const & i) {};
void formast::Visitor::expr_pos(Expr const & right) {};
void formast::Visitor::expr_neg(Expr const & right) {};
void formast::Visitor::expr_add(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_sub(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_mul(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_div(Expr const & left, Expr const & right) {};
