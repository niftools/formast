#include <cassert>
#include <boost/foreach.hpp>
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
        switch (x.op) {
        case '-':
            return visitor.expr_neg(x.right);
        case '+':
            return visitor.expr_pos(x.right);
        default:
            throw std::runtime_error("unknown unary operator");
        }
    }

    void operator()(const formast::detail::ast::binary_op & x) {
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

class formast::Visitor::TopVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    TopVisitor(TopVisitor const &);
    TopVisitor & operator=(TopVisitor const &);

public:
    typedef void result_type;

    TopVisitor(Visitor & visitor) : visitor(visitor) {};

    void top(formast::detail::ast::Top const & t) {
        BOOST_FOREACH(formast::detail::ast::TopDecl const & decl, t) {
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

    void stats(formast::detail::ast::Stats const & s) {
        BOOST_FOREACH(formast::detail::ast::StatDecl const & decl, s) {
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
