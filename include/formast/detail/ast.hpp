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
struct IfElifsElse;

namespace detail
{
namespace ast
{

// declaration nodes

typedef boost::make_recursive_variant<formast::Class>::type TopDecl;

typedef boost::make_recursive_variant<formast::Attr, formast::IfElifsElse>::type StatDecl;

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
    binary_op(char op, Expr const & left, Expr const & right)
        : op(op), left(left), right(right) {}

    char op;
    Expr left;
    Expr right;
};

struct unary_op {
    unary_op(char op, Expr const & right)
        : op(op), right(right) {}

    char op;
    Expr right;
};

}
}
}

#endif
