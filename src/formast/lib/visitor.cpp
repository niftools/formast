#include <cassert>
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <stdexcept>

#include "formast.hpp"
#include "enum_stats_impl.hpp"
#include "expr_impl.hpp"
#include "stats_impl.hpp"
#include "module_impl.hpp"

class formast::Visitor::ExprVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    ExprVisitor(ExprVisitor const &);
    ExprVisitor & operator=(ExprVisitor const &);

public:
    typedef void result_type;

    ExprVisitor(Visitor & visitor) : visitor(visitor) {};

    void expr(Expr const & e) {
        assert(e._impl->tree != 0);
        return boost::apply_visitor(*this, *e._impl->tree);
    }

    void operator()(boost::uint64_t const& n) {
        return visitor.expr_uint(n);
    }

    void operator()(std::string const& i) {
        return visitor.expr_id(i);
    }

    void operator()(const formast::detail::unary_op & x) {
        switch (x.op_type) {
        case formast::detail::unary_op::neg:
            return visitor.expr_neg(x.right);
        case formast::detail::unary_op::pos:
            return visitor.expr_pos(x.right);
        case formast::detail::unary_op::logical_not:
            return visitor.expr_logical_not(x.right);
        }
    }

    void operator()(const formast::detail::binary_op & x) {
        switch (x.op_type) {
        case formast::detail::binary_op::plus:
            return visitor.expr_add(x.left, x.right);
        case formast::detail::binary_op::minus:
            return visitor.expr_sub(x.left, x.right);
        case formast::detail::binary_op::times:
            return visitor.expr_mul(x.left, x.right);
        case formast::detail::binary_op::divide:
            return visitor.expr_div(x.left, x.right);
        case formast::detail::binary_op::mod:
            return visitor.expr_mod(x.left, x.right);
        case formast::detail::binary_op::pow:
            return visitor.expr_pow(x.left, x.right);
        case formast::detail::binary_op::logical_and:
            return visitor.expr_logical_and(x.left, x.right);
        case formast::detail::binary_op::logical_or:
            return visitor.expr_logical_or(x.left, x.right);
        case formast::detail::binary_op::bit_and:
            return visitor.expr_bitwise_and(x.left, x.right);
        case formast::detail::binary_op::bit_or:
            return visitor.expr_bitwise_or(x.left, x.right);
        case formast::detail::binary_op::bit_xor:
            return visitor.expr_bitwise_xor(x.left, x.right);
        case formast::detail::binary_op::equal:
            return visitor.expr_compare_eq(x.left, x.right);
        case formast::detail::binary_op::not_equal:
            return visitor.expr_compare_ne(x.left, x.right);
        case formast::detail::binary_op::greater:
            return visitor.expr_compare_gt(x.left, x.right);
        case formast::detail::binary_op::less:
            return visitor.expr_compare_lt(x.left, x.right);
        case formast::detail::binary_op::greater_equal:
            return visitor.expr_compare_ge(x.left, x.right);
        case formast::detail::binary_op::less_equal:
            return visitor.expr_compare_le(x.left, x.right);
        case formast::detail::binary_op::shift_left:
            return visitor.expr_shift_left(x.left, x.right);
        case formast::detail::binary_op::shift_right:
            return visitor.expr_shift_right(x.left, x.right);
        }
    }

    Visitor & visitor;

};

class formast::Visitor::ModuleVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    ModuleVisitor(ModuleVisitor const &);
    ModuleVisitor & operator=(ModuleVisitor const &);

public:
    typedef void result_type;

    ModuleVisitor(Visitor & visitor) : visitor(visitor) {};

    void module(formast::Module const & t) {
        BOOST_FOREACH(formast::detail::ModuleDecl const & decl, *t._impl) {
            boost::apply_visitor(*this, decl);
        }
    }

    void operator()(formast::Class const & class_) {
        return visitor.module_class(class_);
    }

    void operator()(formast::Enum const & enum_) {
        return visitor.module_enum(enum_);
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

    void operator()(formast::Field const & field) {
        visitor.stats_field(field);
    }

    void operator()(formast::If const & if_) {
        visitor.stats_if(if_);
    }

    Visitor & visitor;

};

class formast::Visitor::EnumStatsVisitor
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    EnumStatsVisitor(EnumStatsVisitor const &);
    EnumStatsVisitor & operator=(EnumStatsVisitor const &);

public:
    typedef void result_type;

    EnumStatsVisitor(Visitor & visitor) : visitor(visitor) {};

    void enum_stats(formast::EnumStats const & s) {
        BOOST_FOREACH(formast::detail::EnumStatDecl const & decl, *s._impl) {
            boost::apply_visitor(*this, decl);
        }
    }

    void operator()(formast::EnumConst const & const_) {
        visitor.enum_stats_const(const_);
    }

    Visitor & visitor;

};

formast::Visitor::Visitor()
{
    _expr_visitor = std::shared_ptr<ExprVisitor>(new ExprVisitor(*this));
    _module_visitor = std::shared_ptr<ModuleVisitor>(new ModuleVisitor(*this));
    _stats_visitor = std::shared_ptr<StatsVisitor>(new StatsVisitor(*this));
    _enum_stats_visitor = std::shared_ptr<EnumStatsVisitor>(new EnumStatsVisitor(*this));
}

formast::Visitor::~Visitor()
{
}

void formast::Visitor::expr(Expr const & e)
{
    _expr_visitor->expr(e);
}

void formast::Visitor::module(Module const & module)
{
    _module_visitor->module(module);
}

void formast::Visitor::stats(Stats const & stats)
{
    _stats_visitor->stats(stats);
}

void formast::Visitor::enum_stats(EnumStats const & enum_stats)
{
    _enum_stats_visitor->enum_stats(enum_stats);
}

void formast::Visitor::module_class(Class const & class_)
{
    if (class_.stats) {
        stats(class_.stats.get());
    }
}

void formast::Visitor::module_enum(Enum const & enum_)
{
    if (enum_.stats) {
        enum_stats(enum_.stats.get());
    }
}

void formast::Visitor::stats_field(Field const & field)
{
    if (field.arr1) {
        expr(field.arr1.get());
    }
    if (field.arr2) {
        expr(field.arr2.get());
    }
}

void formast::Visitor::stats_if(If const & if_)
{
    expr(if_.expr);
    stats(if_.then);
    if (if_.else_) {
        stats(if_.else_.get());
    }
}

void formast::Visitor::enum_stats_const(EnumConst const & const_) {};

void formast::Visitor::expr_uint(boost::uint64_t const & n) {};

void formast::Visitor::expr_id(std::string const & i) {};

void formast::Visitor::expr_pos(Expr const & right)
{
    expr(right);
}

void formast::Visitor::expr_neg(Expr const & right)
{
    expr(right);
}

void formast::Visitor::expr_add(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_sub(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_mul(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_div(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_mod(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_pow(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_logical_and(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_logical_or(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_logical_not(Expr const & right)
{
    expr(right);
}

void formast::Visitor::expr_bitwise_and(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_bitwise_or(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_bitwise_xor(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_compare_eq(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_compare_ne(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_compare_gt(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_compare_lt(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_compare_ge(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_compare_le(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_shift_left(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}

void formast::Visitor::expr_shift_right(Expr const & left, Expr const & right)
{
    expr(left);
    expr(right);
}
