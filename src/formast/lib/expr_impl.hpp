// for friend classes: their cpp files can use this header to access
// implementation

#ifndef FORMAST_EXPR_IMPL_HPP_INCLUDED
#define FORMAST_EXPR_IMPL_HPP_INCLUDED

#include <boost/variant/recursive_variant.hpp>
#include <memory>

#include "formast.hpp"

namespace formast
{
namespace detail
{

struct binary_op;
struct unary_op;

typedef boost::variant<boost::uint64_t, std::string,
        boost::recursive_wrapper<binary_op>,
        boost::recursive_wrapper<unary_op>
        >
        ExprTree;

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

class formast::Expr::Impl
{
private:
    // see http://www.boost.org/doc/libs/1_51_0/libs/smart_ptr/sp_techniques.html#pimpl
    Impl(Impl const &);
    Impl & operator=(Impl const &);
public:
    Impl();

    template <typename T>
    static std::shared_ptr<const formast::detail::ExprTree> create(T const & value) {
        return
            std::shared_ptr<const formast::detail::ExprTree>(
                new formast::detail::ExprTree(value));
    };

    std::shared_ptr<const formast::detail::ExprTree> tree;
};

#endif
