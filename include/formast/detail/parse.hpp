#ifndef FORMAST_DETAIL_PARSE_HPP_INCLUDED
#define FORMAST_DETAIL_PARSE_HPP_INCLUDED

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
// #define BOOST_SPIRIT_QI_DEBUG

#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "formast/detail/ast.hpp"

namespace formast
{
namespace detail
{
namespace parse
{

// a few shorthands

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace ast = formast::detail::ast;

// phoenix functions for constructing the abstract syntax tree with
// semantic actions

template <typename T> // T is a terminal type, i.e. uint64_t or std::string
struct assign_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & left, const T & right) const {
        left = ast::Expr(new ast::ExprNode(right));
    }
};

template <char Op>
struct binary_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & left, ast::Expr const & right) const {
        left = ast::Expr(new ast::ExprNode(ast::binary_op(Op, left, right)));
    }
};

template <char Op>
struct unary_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & right) const {
        right = ast::Expr(new ast::ExprNode(ast::unary_op(Op, right)));
    }
};

boost::phoenix::function<binary_func<'+'> > const add;
boost::phoenix::function<binary_func<'-'> > const sub;
boost::phoenix::function<binary_func<'*'> > const mul;
boost::phoenix::function<binary_func<'/'> > const div;
boost::phoenix::function<unary_func<'+'> > const pos;
boost::phoenix::function<unary_func<'-'> > const neg;
boost::phoenix::function<assign_func<boost::uint64_t> > const assign_uint;

// error handler

struct error_handler_ {
    template <typename, typename, typename>
    struct result {
        typedef void type;
    };

    template <typename Iterator>
    void operator()(
        qi::info const& what, Iterator err_pos, Iterator last) const {
        std::cerr
                << "Error! Expecting "
                << what
                << " here: \""
                << std::string(err_pos, last)
                << "\""
                << std::endl
                ;
    }
};

boost::phoenix::function<error_handler_> const error_handler = error_handler_();

// the actual grammar

template <typename Iterator>
struct parser : qi::grammar<Iterator, ast::Expr(), ascii::space_type> {

    parser() : parser::base_type(expr) {

    qi::char_type char_;
    qi::uint_type uint_;
    qi::_val_type _val;
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    using qi::on_error;
    using qi::fail;

    expr =
        term                            [_val = _1]
        >> *(   ('+' > term             [add(_val, _1)])
                |   ('-' > term         [sub(_val, _1)])
            )
        ;

    term =
        factor                          [_val = _1]
        >> *(   ('*' > factor           [mul(_val, _1)])
                |   ('/' > factor       [div(_val, _1)])
            )
        ;

    factor =
        uint_                           [assign_uint(_val, _1)]
        |   '(' > expr                  [_val = _1] > ')'
        |   ('-' > factor               [neg(_1)])
        |   ('+' > factor               [pos(_1)])
        ;

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODE(expr);
    BOOST_SPIRIT_DEBUG_NODE(term);
    BOOST_SPIRIT_DEBUG_NODE(factor);

    // Error handling
    on_error<fail>(expr, error_handler(_4, _3, _2));
}

qi::rule<Iterator, ast::Expr(), ascii::space_type> expr, term, factor;
};


}
}
}

#endif
