#include "formast.hpp"
#include "expr_impl.hpp"

formast::Expr::Impl::Impl()
    : tree()
{
}

formast::Expr::Expr()
    : _impl(new Impl)
{
}
