#ifndef FORMAST_DETAIL_AST_HPP_INCLUDED
#define FORMAST_DETAIL_AST_HPP_INCLUDED

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant/recursive_variant.hpp>

// this header file provides an implementation of the abstract syntax tree

namespace formast
{

// forward declarations
// these are defined in formast.hpp

struct Attr;
struct Class;
struct If;

namespace detail
{
namespace ast
{

// declaration nodes

typedef boost::make_recursive_variant<formast::Class>::type TopDecl;

typedef boost::make_recursive_variant<formast::Attr, formast::If>::type StatDecl;

typedef std::vector<TopDecl> Top;
typedef std::vector<StatDecl> Stats;

// expression nodes

struct binary_op;
struct unary_op;

typedef boost::variant<boost::uint64_t, std::string,
        boost::recursive_wrapper<binary_op>,
        boost::recursive_wrapper<unary_op>
        >
        ExprNode;

typedef boost::shared_ptr<const ExprNode> Expr;

struct binary_op {

    enum type {
        logical_or,
        logical_and,
        bit_or,
        bit_xor,
        bit_and,
        equal,
        not_equal,
        less,
        less_equal,
        greater,
        greater_equal,
        shift_left,
        shift_right,
        plus,
        minus,
        times,
        divide,
        mod,
        pow,
    };

    binary_op(type op_type, Expr const & left, Expr const & right)
        : op_type(op_type), left(left), right(right) {}

    type op_type;
    Expr left;
    Expr right;
};

struct unary_op {

    enum type {
        neg,
        pos,
        logical_not,
    };

    unary_op(type op_type, Expr const & right)
        : op_type(op_type), right(right) {}

    type op_type;
    Expr right;
};

}
}
}

#endif
