#include "parser_impl.hpp"

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
// #define BOOST_SPIRIT_QI_DEBUG

// shorthand

namespace qi = boost::spirit::qi;

// expression grammar implementation

formast::Parser::Impl::expr_grammar::expr_grammar() : expr_grammar::base_type(expr)
{

    boost::phoenix::function<binary_func<detail::binary_op::logical_or> > const _logical_or;
    boost::phoenix::function<binary_func<detail::binary_op::logical_and> > const _logical_and;
    boost::phoenix::function<binary_func<detail::binary_op::bit_or> > const _bit_or;
    boost::phoenix::function<binary_func<detail::binary_op::bit_xor> > const _bit_xor;
    boost::phoenix::function<binary_func<detail::binary_op::bit_and> > const _bit_and;
    boost::phoenix::function<binary_func<detail::binary_op::equal> > const _equal;
    boost::phoenix::function<binary_func<detail::binary_op::not_equal> > const _not_equal;
    boost::phoenix::function<binary_func<detail::binary_op::less> > const _less;
    boost::phoenix::function<binary_func<detail::binary_op::less_equal> > const _less_equal;
    boost::phoenix::function<binary_func<detail::binary_op::greater> > const _greater;
    boost::phoenix::function<binary_func<detail::binary_op::greater_equal> > const _greater_equal;
    boost::phoenix::function<binary_func<detail::binary_op::shift_left> > const _shift_left;
    boost::phoenix::function<binary_func<detail::binary_op::shift_right> > const _shift_right;
    boost::phoenix::function<binary_func<detail::binary_op::plus> > const _add;
    boost::phoenix::function<binary_func<detail::binary_op::minus> > const _sub;
    boost::phoenix::function<binary_func<detail::binary_op::times> > const _mul;
    boost::phoenix::function<binary_func<detail::binary_op::divide> > const _div;
    boost::phoenix::function<binary_func<detail::binary_op::mod> > const _mod;
    boost::phoenix::function<binary_func<detail::binary_op::pow> > const _pow;
    boost::phoenix::function<unary_func<detail::unary_op::pos> > const _pos;
    boost::phoenix::function<unary_func<detail::unary_op::neg> > const _neg;
    boost::phoenix::function<unary_func<detail::unary_op::logical_not> > const _logical_not;
    boost::phoenix::function<assign_func<std::string> > const _ident;
    boost::phoenix::function<assign_func<boost::uint64_t> > const _uint;
    boost::phoenix::function<copy_func> const _copy;
    boost::phoenix::function<trim_func> const _trim;

    qi::lexeme_type lexeme;
    qi::lit_type lit;
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
        |   ('(' > expr                 [_copy(_val, _1)] > ')')
        ;

    // also match trailing whitespace; _trim removes it
    ident_ws %= lexeme[(char_("a-zA-Z") >> *(char_(" ?") | char_("0-9a-zA-Z")))];
    ident = ident_ws [_trim(_val, _1)];

    uint_or_version %=
        (lit("2.3")       [_val = 0x02030000])
        |   (lit("3.03")  [_val = 0x03000300]) // special case
        |   (lit("3.0")   [_val = 0x03000000])
        |   (lit("3.1")   [_val = 0x03010000])
        |   ((ulong_long                [_val = _1])
             >> -('.' >> (ulong_long    [_val = _val * 256 + _1])
                  >> '.' > (ulong_long  [_val = _val * 256 + _1])
                  > '.' > (ulong_long   [_val = _val * 256 + _1])))
        ;

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODE(expr);
    BOOST_SPIRIT_DEBUG_NODE(term);
    BOOST_SPIRIT_DEBUG_NODE(factor);

    // Error handling
    on_error<fail>(expr, error_handler(_4, _3, _2));
}
