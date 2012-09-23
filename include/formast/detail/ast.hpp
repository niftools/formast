#ifndef FORMAST_DETAIL_AST_HPP_INCLUDED
#define FORMAST_DETAIL_AST_HPP_INCLUDED

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <list>
#include <string>
#include <vector>

// this header file provides an implementation of the abstract syntax tree

namespace formast
{
namespace detail
{
namespace ast
{

// documentation node

typedef std::list<std::string> Doc;

// declaration node

struct Attr;
struct Class;
struct IfElifsElse;
typedef boost::make_recursive_variant<Class, Attr, IfElifsElse>::type Declaration;

// scope node

typedef std::vector<Declaration> Scope;

// attribute node

struct Attr
{
    std::string class_name;
    std::string name;
    boost::optional<Doc> doc;
};

// class node

struct Class
{
    std::string name;
    boost::optional<std::string> base_name;
    boost::optional<Doc> doc;
    boost::optional<Scope> scope;
};

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

// conditional nodes

struct If
{
    Expr expr;
    Scope scope;
};

class IfElifsElse
{
    std::vector<If> ifs_;
    boost::optional<Scope> else_;
};

}
}
}

// upgrade structs to fusion sequences

BOOST_FUSION_ADAPT_STRUCT(
    formast::detail::ast::Attr,
    (std::string, class_name)
    (std::string, name)
    (boost::optional<formast::detail::ast::Doc>, doc)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::detail::ast::Class,
    (std::string, name)
    (boost::optional<std::string>, base_name)
    (boost::optional<formast::detail::ast::Doc>, doc)
    (boost::optional<formast::detail::ast::Scope>, scope)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::detail::ast::If,
    (formast::detail::ast::Expr, expr)
    (formast::detail::ast::Scope, scope)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::detail::ast::IfElifsElse,
    (std::vector<formast::detail::ast::If>, ifs_)
    (boost::optional<formast::detail::ast::Scope>, else_)
)

#endif
