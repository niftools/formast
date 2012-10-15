#include <cassert>
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <stdexcept>

#include "formast.hpp"
#include "stats_impl.hpp"
#include "top_impl.hpp"

class formast::Visitor::ExprVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    ExprVisitor(ExprVisitor const &);
    ExprVisitor & operator=(ExprVisitor const &);

public:
    typedef void result_type;

    ExprVisitor(Visitor & visitor) : visitor(visitor) {};

    void expr(formast::detail::ast::Expr const & e) {
        assert(e != 0);
        return boost::apply_visitor(*this, *e);
    }

    void operator()(boost::uint64_t const& n) {
        return visitor.expr_uint(n);
    }

    void operator()(std::string const& i) {
        return visitor.expr_id(i);
    }

    void operator()(const formast::detail::ast::unary_op & x) {
        switch (x.op_type) {
        case formast::detail::ast::unary_op::neg:
            return visitor.expr_neg(x.right);
        case formast::detail::ast::unary_op::pos:
            return visitor.expr_pos(x.right);
        case formast::detail::ast::unary_op::logical_not:
            return visitor.expr_logical_not(x.right);
        }
    }

    void operator()(const formast::detail::ast::binary_op & x) {
        switch (x.op_type) {
        case formast::detail::ast::binary_op::plus:
            return visitor.expr_add(x.left, x.right);
        case formast::detail::ast::binary_op::minus:
            return visitor.expr_sub(x.left, x.right);
        case formast::detail::ast::binary_op::times:
            return visitor.expr_mul(x.left, x.right);
        case formast::detail::ast::binary_op::divide:
            return visitor.expr_div(x.left, x.right);
        case formast::detail::ast::binary_op::mod:
            return visitor.expr_mod(x.left, x.right);
        case formast::detail::ast::binary_op::pow:
            return visitor.expr_pow(x.left, x.right);
        case formast::detail::ast::binary_op::logical_and:
            return visitor.expr_logical_and(x.left, x.right);
        case formast::detail::ast::binary_op::logical_or:
            return visitor.expr_logical_or(x.left, x.right);
        case formast::detail::ast::binary_op::bit_and:
            return visitor.expr_bitwise_and(x.left, x.right);
        case formast::detail::ast::binary_op::bit_or:
            return visitor.expr_bitwise_or(x.left, x.right);
        case formast::detail::ast::binary_op::bit_xor:
            return visitor.expr_bitwise_xor(x.left, x.right);
        case formast::detail::ast::binary_op::equal:
            return visitor.expr_compare_eq(x.left, x.right);
        case formast::detail::ast::binary_op::not_equal:
            return visitor.expr_compare_ne(x.left, x.right);
        case formast::detail::ast::binary_op::greater:
            return visitor.expr_compare_gt(x.left, x.right);
        case formast::detail::ast::binary_op::less:
            return visitor.expr_compare_lt(x.left, x.right);
        case formast::detail::ast::binary_op::greater_equal:
            return visitor.expr_compare_ge(x.left, x.right);
        case formast::detail::ast::binary_op::less_equal:
            return visitor.expr_compare_le(x.left, x.right);
        case formast::detail::ast::binary_op::shift_left:
            return visitor.expr_shift_left(x.left, x.right);
        case formast::detail::ast::binary_op::shift_right:
            return visitor.expr_shift_right(x.left, x.right);
        }
    }

    Visitor & visitor;

};

class formast::Visitor::TopVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    TopVisitor(TopVisitor const &);
    TopVisitor & operator=(TopVisitor const &);

public:
    typedef void result_type;

    TopVisitor(Visitor & visitor) : visitor(visitor) {};

    void top(formast::Top const & t) {
        BOOST_FOREACH(formast::detail::TopDecl const & decl, *t._impl) {
            boost::apply_visitor(*this, decl);
        }
    }

    void operator()(formast::Class const & class_) {
        return visitor.top_class(class_);
    }

    Visitor & visitor;

};

class formast::Visitor::StatsVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    StatsVisitor(StatsVisitor const &);
    StatsVisitor & operator=(StatsVisitor const &);

public:
    typedef void result_type;

    StatsVisitor(Visitor & visitor) : visitor(visitor) {};

    void stats(formast::Stats const & s) {
        BOOST_FOREACH(formast::detail::StatDecl const & decl, *s._impl) {
            boost::apply_visitor(*this, decl);
        }
    }

    void operator()(formast::Attr const & attr) {
        visitor.stats_attr(attr);
    }

    void operator()(formast::If const & if_) {
        visitor.stats_if(if_);
    }

    Visitor & visitor;

};

formast::Visitor::Visitor()
{
    _expr_visitor = boost::shared_ptr<ExprVisitor>(new ExprVisitor(*this));
    _top_visitor = boost::shared_ptr<TopVisitor>(new TopVisitor(*this));
    _stats_visitor = boost::shared_ptr<StatsVisitor>(new StatsVisitor(*this));
}

formast::Visitor::~Visitor()
{
}

void formast::Visitor::expr(Expr const & e)
{
    _expr_visitor->expr(e);
}

void formast::Visitor::top(Top const & top)
{
    _top_visitor->top(top);
};

void formast::Visitor::stats(Stats const & stats)
{
    _stats_visitor->stats(stats);
};

void formast::Visitor::top_class(Class const & class_) {};

void formast::Visitor::stats_attr(Attr const & attr) {};
void formast::Visitor::stats_if(If const & if_) {};

void formast::Visitor::expr_uint(boost::uint64_t const & n) {};
void formast::Visitor::expr_id(std::string const & i) {};
void formast::Visitor::expr_pos(Expr const & right) {};
void formast::Visitor::expr_neg(Expr const & right) {};
void formast::Visitor::expr_add(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_sub(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_mul(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_div(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_mod(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_pow(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_logical_and(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_logical_or(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_logical_not(Expr const & right) {};
void formast::Visitor::expr_bitwise_and(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_bitwise_or(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_bitwise_xor(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_compare_eq(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_compare_ne(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_compare_gt(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_compare_lt(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_compare_ge(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_compare_le(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_shift_left(Expr const & left, Expr const & right) {};
void formast::Visitor::expr_shift_right(Expr const & left, Expr const & right) {};
