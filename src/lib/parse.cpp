#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
// #define BOOST_SPIRIT_QI_DEBUG

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
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
    (boost::optional<formast::Stats>, stats)
)

BOOST_FUSION_ADAPT_STRUCT(
    formast::If,
    (formast::Expr, expr)
    (formast::Stats, then)
    (boost::optional<formast::Stats>, else_)
)

// a few shorthands

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace ast = formast::detail::ast;

// phoenix functions for constructing the abstract syntax tree with
// semantic actions

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

struct trim_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(std::string & left, std::string const & right) const {
        left = right;
        boost::algorithm::trim(left);
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

template <ast::binary_op::type op_type>
struct binary_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & left, ast::Expr const & right) const {
        assert(left != 0);
        assert(right != 0);
        left = ast::Expr(new ast::ExprNode(ast::binary_op(op_type, left, right)));
    }
};

template <ast::unary_op::type op_type>
struct unary_func {
    template <typename T1, typename T2 = void>
    struct result {
        typedef void type;
    };

    void operator()(ast::Expr & left, ast::Expr & right) const {
        assert(right != 0);
        left = ast::Expr(new ast::ExprNode(ast::unary_op(op_type, right)));
    }
};

boost::phoenix::function<binary_func<ast::binary_op::logical_or> > const _logical_or;
boost::phoenix::function<binary_func<ast::binary_op::logical_and> > const _logical_and;
boost::phoenix::function<binary_func<ast::binary_op::bit_or> > const _bit_or;
boost::phoenix::function<binary_func<ast::binary_op::bit_xor> > const _bit_xor;
boost::phoenix::function<binary_func<ast::binary_op::bit_and> > const _bit_and;
boost::phoenix::function<binary_func<ast::binary_op::equal> > const _equal;
boost::phoenix::function<binary_func<ast::binary_op::not_equal> > const _not_equal;
boost::phoenix::function<binary_func<ast::binary_op::less> > const _less;
boost::phoenix::function<binary_func<ast::binary_op::less_equal> > const _less_equal;
boost::phoenix::function<binary_func<ast::binary_op::greater> > const _greater;
boost::phoenix::function<binary_func<ast::binary_op::greater_equal> > const _greater_equal;
boost::phoenix::function<binary_func<ast::binary_op::shift_left> > const _shift_left;
boost::phoenix::function<binary_func<ast::binary_op::shift_right> > const _shift_right;
boost::phoenix::function<binary_func<ast::binary_op::plus> > const _add;
boost::phoenix::function<binary_func<ast::binary_op::minus> > const _sub;
boost::phoenix::function<binary_func<ast::binary_op::times> > const _mul;
boost::phoenix::function<binary_func<ast::binary_op::divide> > const _div;
boost::phoenix::function<binary_func<ast::binary_op::mod> > const _mod;
boost::phoenix::function<binary_func<ast::binary_op::pow> > const _pow;
boost::phoenix::function<unary_func<ast::unary_op::pos> > const _pos;
boost::phoenix::function<unary_func<ast::unary_op::neg> > const _neg;
boost::phoenix::function<unary_func<ast::unary_op::logical_not> > const _logical_not;
boost::phoenix::function<assign_func<std::string> > const _ident;
boost::phoenix::function<assign_func<boost::uint64_t> > const _uint;
boost::phoenix::function<copy_func> const _copy;
boost::phoenix::function<trim_func> const _trim;

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
struct expr_grammar : qi::grammar<Iterator, ast::Expr(), ascii::space_type> {

    expr_grammar() : expr_grammar::base_type(expr) {

    qi::lexeme_type lexeme;
    qi::char_type char_;
    qi::uint_type ulong_long;
    qi::_val_type _val;
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    using qi::on_error;
    using qi::fail;

    expr = or_test.alias();

    or_test =
        and_test                        [_copy(_val, _1)]
        >> *("||" > and_test            [_logical_or(_val, _1)])
        ;

    and_test =
        not_test                        [_copy(_val, _1)]
        >> *("&&" > not_test            [_logical_and(_val, _1)])
        ;

    not_test =
        ('!' >> not_test                [_logical_not(_val, _1)])
        |   comparison                  [_copy(_val, _1)]
        ;

    comparison =
        bit_or_expr                     [_copy(_val, _1)]
        >> *(   ('<' >> bit_or_expr     [_less(_val, _1)])
                |   ('>' >> bit_or_expr [_greater(_val, _1)])
                |   ("==" > bit_or_expr [_equal(_val, _1)])
                |   ("!=" > bit_or_expr [_not_equal(_val, _1)])
                |   (">=" > bit_or_expr [_greater_equal(_val, _1)])
                |   ("<=" > bit_or_expr [_less_equal(_val, _1)])
            )
        ;

    bit_or_expr =
        bit_xor_expr                    [_copy(_val, _1)]
        >> *('|' >> bit_xor_expr        [_bit_or(_val, _1)])
        ;

    bit_xor_expr =
        bit_and_expr                    [_copy(_val, _1)]
        >> *('^' > bit_and_expr         [_bit_xor(_val, _1)])
        ;

    bit_and_expr =
        bit_shift_expr                  [_copy(_val, _1)]
        >> *('&' >> bit_shift_expr      [_bit_and(_val, _1)])
        ;

    bit_shift_expr =
        arith_expr                      [_copy(_val, _1)]
        >> *(   (">>" > arith_expr      [_shift_right(_val, _1)])
                |   ("<<" > arith_expr  [_shift_left(_val, _1)])
            )
        ;

    arith_expr =
        term                            [_copy(_val, _1)]
        >> *(   ('+' > term             [_add(_val, _1)])
                |   ('-' > term         [_sub(_val, _1)])
            )
        ;

    term =
        factor                          [_copy(_val, _1)]
        >> *(   ('*' > factor           [_mul(_val, _1)])
                |   ('/' > factor       [_div(_val, _1)])
                |   ('%' > factor       [_mod(_val, _1)])
            )
        ;

    factor =
        power                           [_copy(_val, _1)]
        |   ('-' > factor               [_neg(_val, _1)])
        |   ('+' > factor               [_pos(_val, _1)])
        ;

    power =
        atom                            [_copy(_val, _1)]
        >> -("**" > factor              [_pow(_val, _1)])
        ;

    atom =
        uint_or_version                 [_uint(_val, _1)]
        |   ident                       [_ident(_val, _1)]
        |   '(' > expr                  [_copy(_val, _1)] > ')'
        ;

    // also match trailing whitespace; _trim removes it
    ident_ws %= lexeme[(char_("a-zA-Z") >> *(char_(" ") | char_("0-9a-zA-Z")))];
    ident = ident_ws [_trim(_val, _1)];

    uint_or_version %=
        (ulong_long                [_val = _1])
        >> -('.' >> (ulong_long    [_val = _val * 256 + _1])
             >> '.' > (ulong_long  [_val = _val * 256 + _1])
             > '.' > (ulong_long   [_val = _val * 256 + _1]))
        ;

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODE(expr);
    BOOST_SPIRIT_DEBUG_NODE(term);
    BOOST_SPIRIT_DEBUG_NODE(factor);

    // Error handling
    on_error<fail>(expr, error_handler(_4, _3, _2));
}

qi::rule<Iterator, ast::Expr(), ascii::space_type> expr, or_test, and_test, not_test, comparison, bit_or_expr, bit_xor_expr, bit_and_expr, bit_shift_expr, arith_expr, term, factor, power, atom;
qi::rule<Iterator, std::string(), ascii::space_type> ident, ident_ws;
qi::rule<Iterator, boost::uint64_t(), ascii::space_type> uint_or_version;
};

// helper function for parsing expression from stream
void _expr_parse_stream(std::istream & is, ast::Expr & e)
{
    // disable white space skipping
    is.unsetf(std::ios::skipws);
    typedef boost::spirit::istream_iterator iterator_type;
    iterator_type iter(is);
    iterator_type end;
    expr_grammar<iterator_type> grammar;
    ascii::space_type space;

    bool success = qi::phrase_parse(iter, end, grammar, space, e);
    if (!success) {
        throw std::runtime_error("Syntax error.");
    }
    if (iter != end) {
        throw std::runtime_error("End of stream not reached.");
    }
}

// helper function for parsing expression from string
void _expr_parse_string(std::string const & s, ast::Expr & e)
{
    std::istringstream is(s);
    _expr_parse_stream(is, e);
}

formast::Parser::Parser()
{
}

void formast::Parser::parse_string(std::string const & s, ast::Top & top)
{
    std::istringstream is(s);
    parse_stream(is, top);
}

formast::XmlParser::XmlParser()
    : Parser()
{
}

void formast::XmlParser::parse_stream(std::istream & is, ast::Top & top)
{
    // disable skipping of whitespace
    is.unsetf(std::ios::skipws);

    // read xml into property tree
    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(is, pt);
    //boost::property_tree::info_parser::write_info(std::cout, pt); // DEBUG

    BOOST_FOREACH(boost::property_tree::ptree::value_type & decl, pt.get_child("niftoolsxml")) {
        if (decl.first == "basic") {
            Class class_;
            class_.name = decl.second.get<std::string>("<xmlattr>.name");
            std::string doc = decl.second.data();
            boost::algorithm::trim(doc);
            if (!doc.empty()) {
                class_.doc = doc;
            }
            top.push_back(class_);
        } else if (decl.first == "compound" || decl.first == "niobject") {
            Class class_;
            class_.name = decl.second.get<std::string>("<xmlattr>.name");
            std::string doc = decl.second.data();
            boost::algorithm::trim(doc);
            if (!doc.empty()) {
                class_.doc = doc;
            }
            class_.base_name = decl.second.get_optional<std::string>("<xmlattr>.inherit");
            ast::Stats stats;
            BOOST_FOREACH(boost::property_tree::ptree::value_type & add, decl.second) {
                if (add.first == "add") {
                    Attr attr;
                    attr.class_name = add.second.get<std::string>("<xmlattr>.type");
                    attr.name = add.second.get<std::string>("<xmlattr>.name");
                    std::string doc = add.second.data();
                    boost::algorithm::trim(doc);
                    if (!doc.empty()) {
                        attr.doc = doc;
                    }
                    // conditioning disabled for now, can't parse it completely yet
                    boost::optional<std::string> cond = add.second.get_optional<std::string>("<xmlattr>.cond");
                    if  (!cond) {
                        stats.push_back(attr);
                    } else {
                        formast::If if_;
                        _expr_parse_string(cond.get(), if_.expr);
                        if_.then.push_back(attr);
                        stats.push_back(if_);
                    }
                };
            };
            if (!stats.empty()) {
                class_.stats = stats;
            };
            top.push_back(class_);
        };
    };
}
