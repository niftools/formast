#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
// #define BOOST_SPIRIT_QI_DEBUG

#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <cassert>
#include <sstream>

#include "formast.hpp"

// upgrade structs to fusion sequences

BOOST_FUSION_ADAPT_STRUCT(
    formast::Attr,
    (std::string, class_name)
    (std::string, name)
    (boost::optional<formast::Doc>, doc)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::Class,
    (std::string, name)
    (boost::optional<std::string>, base_name)
    (boost::optional<formast::Doc>, doc)
    (boost::optional<formast::Scope>, scope)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::If,
    (formast::Expr, expr)
    (formast::Scope, scope)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::IfElifsElse,
    (std::vector<formast::If>, ifs_)
    (boost::optional<formast::Scope>, else_)
)

// a few shorthands

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace ast = formast::detail::ast;

// phoenix functions for constructing the abstract syntax tree with
// semantic actions

template <typename T> // T is a terminal type, i.e. uint64_t or std::string
struct copy_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & left, ast::Expr const & right) const {
        assert(right != 0);
        left = right;
    }
};

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
        assert(left != 0);
        assert(right != 0);
        left = ast::Expr(new ast::ExprNode(ast::binary_op(Op, left, right)));
    }
};

template <char Op>
struct unary_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & left, ast::Expr & right) const {
        assert(right != 0);
        left = ast::Expr(new ast::ExprNode(ast::unary_op(Op, right)));
    }
};

boost::phoenix::function<binary_func<'+'> > const _add;
boost::phoenix::function<binary_func<'-'> > const _sub;
boost::phoenix::function<binary_func<'*'> > const _mul;
boost::phoenix::function<binary_func<'/'> > const _div;
boost::phoenix::function<unary_func<'+'> > const _pos;
boost::phoenix::function<unary_func<'-'> > const _neg;
boost::phoenix::function<assign_func<boost::uint64_t> > const _uint;
boost::phoenix::function<copy_func<boost::uint64_t> > const _copy;

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
        term                            [_copy(_val, _1)]
        >> *(   ('+' > term             [_add(_val, _1)])
                |   ('-' > term         [_sub(_val, _1)])
            )
        ;

    term =
        factor                          [_copy(_val, _1)]
        >> *(   ('*' > factor           [_mul(_val, _1)])
                |   ('/' > factor       [_div(_val, _1)])
            )
        ;

    factor =
        uint_                           [_uint(_val, _1)]
        |   '(' > expr                  [_copy(_val, _1)] > ')'
        |   ('-' > factor               [_neg(_val, _1)])
        |   ('+' > factor               [_pos(_val, _1)])
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

formast::Parser::Parser()
{
}

formast::Expr formast::Parser::parse_string(std::string const & s)
{
    std::istringstream is(s);
    return parse_stream(is);
}

formast::XmlParser::XmlParser()
    : Parser()
{
}

formast::Expr formast::XmlParser::parse_stream(std::istream & is)
{
    // disable white space skipping
    is.unsetf(std::ios::skipws);

    typedef boost::spirit::istream_iterator iterator_type;
    iterator_type iter(is);
    iterator_type end;
    parser<iterator_type> parser;
    ascii::space_type space;
    formast::Expr e;
    bool success = qi::phrase_parse(iter, end, parser, space, e);
    if (!success) {
        throw std::runtime_error("Syntax error.");
    }
    if (iter != end) {
        throw std::runtime_error("End of stream not reached.");
    }
    return e;
}
