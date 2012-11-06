#include "parser_impl.hpp"
#include <boost/spirit/include/qi_uint.hpp>

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
// #define BOOST_SPIRIT_QI_DEBUG

// shorthand

namespace qi = boost::spirit::qi;

// integer grammar implementation

formast::Parser::Impl::integer_grammar::integer_grammar() : integer_grammar::base_type(uint_or_version)
{

    qi::uint_parser<boost::uint64_t, 16> hex_parser;
    qi::lit_type lit;
    qi::uint_type ulong_long;
    qi::_val_type _val;
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    uint_or_version =
        hex
        |   (lit("2.3")   [_val = 0x02030000])
        |   (lit("3.03")  [_val = 0x03000300]) // special case
        |   (lit("3.0")   [_val = 0x03000000])
        |   (lit("3.1")   [_val = 0x03010000])
        |   ((ulong_long                [_val = _1])
             >> -('.' >> (ulong_long    [_val = _val * 256 + _1])
                  >> '.' > (ulong_long  [_val = _val * 256 + _1])
                  > '.' > (ulong_long   [_val = _val * 256 + _1])))
        ;

    hex = lit("0x") > hex_parser;

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODE(uint_or_version);

    // Error handling
    qi::on_error<qi::fail>(uint_or_version, error_handler(_4, _3, _2));
}
