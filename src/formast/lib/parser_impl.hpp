#ifndef FORMAST_PARSER_IMPL_INCLUDED
#define FORMAST_PARSER_IMPL_INCLUDED

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <cassert>

#include "formast.hpp"
#include "expr_impl.hpp"

// parser implementation

class formast::Parser::Impl
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    Impl(Impl const &);
    Impl & operator=(Impl const &);

public:
    Impl();

    typedef boost::spirit::istream_iterator Iterator;
    typedef boost::spirit::ascii::space_type SpaceType;

    // error handler

    struct error_handler_ {
        template <typename, typename, typename>
        struct result {
            typedef void type;
        };

        void operator()(
            boost::spirit::qi::info const& what, Iterator err_pos, Iterator last) const {
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

    static boost::phoenix::function<error_handler_> const error_handler;

    // phoenix functions for constructing the abstract syntax tree with
    // semantic actions

    struct copy_func {
        template <typename T1, typename T2 = void>
        struct result {
            typedef void type;
        };

        void operator()(formast::Expr & left, formast::Expr const & right) const {
            assert(right._impl->tree != 0);
            left._impl->tree = right._impl->tree;
        }
    };

    struct trim_func {
        template <typename T1, typename T2 = void>
        struct result {
            typedef void type;
        };

        void operator()(std::string & left, std::string const & right) const {
            left = boost::algorithm::trim_copy(right);
        }
    };

    template <typename T> // T is a terminal type, i.e. uint64_t or std::string
    struct assign_func {
        template <typename T1, typename T2 = void>
        struct result {
            typedef void type;
        };

        void operator()(formast::Expr & left, const T & right) const {
            left._impl->tree = formast::Expr::Impl::create(right);
        }
    };

    template <detail::binary_op::type op_type>
    struct binary_func {
        template <typename T1, typename T2 = void>
        struct result {
            typedef void type;
        };

        void operator()(formast::Expr & left, formast::Expr const & right) const {
            assert(left._impl->tree != 0);
            assert(right._impl->tree != 0);
            left._impl->tree = formast::Expr::Impl::create(detail::binary_op(op_type, left, right));
        }
    };

    template <detail::unary_op::type op_type>
    struct unary_func {
        template <typename T1, typename T2 = void>
        struct result {
            typedef void type;
        };

        void operator()(formast::Expr & left, formast::Expr & right) const {
            assert(right._impl->tree != 0);
            left._impl->tree = formast::Expr::Impl::create(detail::unary_op(op_type, right));
        }
    };

    // the actual grammar

    struct expr_grammar : boost::spirit::qi::grammar<Iterator, formast::Expr(), SpaceType> {

        expr_grammar();

        boost::spirit::qi::rule<Iterator, formast::Expr(), SpaceType> expr, or_test, and_test, not_test, comparison, bit_or_expr, bit_xor_expr, bit_and_expr, bit_shift_expr, arith_expr, term, factor, power, atom;
        boost::spirit::qi::rule<Iterator, std::string(), SpaceType> ident, ident_ws;
        boost::spirit::qi::rule<Iterator, boost::uint64_t(), SpaceType> uint_or_version;
    };

    // helper function for parsing expression from stream
    void _expr_parse_stream(std::istream & is, formast::Expr & e);

    // helper function for parsing expression from string
    void _expr_xml_parse_string(std::string const & s, formast::Expr & e);

    // parse s
    // if expr contains nothing, assign parsed expression
    // otherwise, && with parsed expression
    void _expr_xml_parse_helper(std::string const & s, formast::Expr & e);

};

#endif
